
#pragma once

#include <MeshLoader.hpp>

#include <filesystem>
#include <vector>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <fileParser.hpp>

constexpr auto BUFFER_SIZE = 16 * 1024;

template<vertex_comp... Cs>
void MeshLoader::loadFromOBJ(
	const std::string& filename, std::vector<mesh<Cs...>>& meshes,
	std::vector<material>& materials
) {

	//temporary storage for parsing
	std::vector<vertex3D> vertices{ { 0.f, 0.f, 0.f } };
	std::vector<vertex3D> normals{ { 0.f, 0.f, 0.f } };
	std::vector<vertex2D> texCoords{ { 0.f, 0.f } };
	
	//the final buffers for the gpu
	std::vector<typename mesh<Cs...>::vertex> vertexBuffer;
	std::vector<u16> indexBuffer;

	//vertex combinations for indexing
	std::vector<indexedVertexID<Cs...>> vertexIDs;

	size_t materialIndex = -1;

	const auto createMesh = [&]() {
		if (vertexBuffer.size() > 0) {
			//auto& newMesh = destination.emplace_back(std::move(vertexBuffer), std::move(indexBuffer));
			auto& newMesh = meshes.emplace_back(vertexBuffer, indexBuffer);

			if (materialIndex != -1) {
				const auto& mtl = materials[materialIndex];
				if (mtl.colorAttribute && !mtl.textureAttribute)
					newMesh.addAttribute(mtl.colorAttribute);
				if (mtl.textureAttribute)
					newMesh.addAttribute(mtl.textureAttribute);
			}	
		}

		vertexBuffer.clear();
		indexBuffer.clear();
		vertexIDs.clear();

		materialIndex = -1;
	};

	namespace fs = std::filesystem;
	const std::string directory = fs::path(filename).parent_path().string() + fs::path::preferred_separator;

	parseFile<BUFFER_SIZE>(filename,
		parser{ "v ", [&](const char* begin, const char* end){
			char* next;
			float x = std::strtof(begin, &next);
			float y = std::strtof(++next, &next);
			float z = std::strtof(++next, &next);
			vertices.emplace_back(x, y, z);
		}},
		parser{ "vt ", [&](const char* begin, const char* end){
			char* next;
			float x = std::strtof(begin, &next);
			float y = std::strtof(++next, &next);
			texCoords.emplace_back(x, y);
		}},
		parser{ "vn ", [&](const char* begin, const char* end){
			char* next;
			float x = std::strtof(begin, &next);
			float y = std::strtof(++next, &next);
			float z = std::strtof(++next, &next);
			normals.emplace_back(x, y, z);
		}},
		parser{ "f ", [&](const char* begin, const char* end){
			u32 firstIndex, prevIndex, compIndex = 0;

			// index for position and all the vertex components
			u32 indices[(1 + ... + sizeof(Cs))];

			// indices are set to 0 so if the obj does not hold data
			// for that component the fallback component at index 0 is used
			std::memset(indices, 0, sizeof(indices));

			size_t charIndex = 0;
			const char* reader = begin;
			for (int vertexindex = 0; reader <= end + 1; ) {
				if (reader == end + 1 || *reader == ' ') {			

					u32 index;

					indexedVertexID<Cs...> vID(indices);
					const auto it = std::upper_bound(vertexIDs.begin(), vertexIDs.end(), vID);

					if (it > vertexIDs.begin() && *(it - 1) == vID) {
						index = (it - 1)->bufferIndex;
					} else {
						
						index = vID.bufferIndex = vertexBuffer.size();
						vertexIDs.insert(it, vID);

						u8* vertexData = vertexBuffer.emplace_back().data;
						size_t byteOffset = 0;

						const auto push = [&vertexData, &byteOffset]<typename T>(const T& value) {
							std::memcpy(vertexData + byteOffset, (u8*)&value, sizeof(T));
							byteOffset += sizeof(T);
						};

						push(vertices[indices[0]]);
					
						if constexpr (any_of<vertex_comps::texCoord, Cs...>)
							push(texCoords[indices[1]]);

						if constexpr (any_of<vertex_comps::normal, Cs...>)
							push(normals[indices[2]]);

					}

					if (vertexindex >= 2) {
						indexBuffer.reserve(3);
						indexBuffer.push_back(firstIndex);
						indexBuffer.push_back(prevIndex);
						indexBuffer.push_back(index);
					} else if (vertexindex == 0) {
						firstIndex = index;
					}
					prevIndex = index;
					vertexindex++;
					compIndex = 0;
					reader++;
				} else if (*reader == '/') {
					compIndex++;
					reader++;
				} else {
					char* next;
					indices[compIndex] = strtol(reader, &next, 10);
					reader = next;
				}
			}
		}},
		parser{ "o ", [&](const char* begin, const char* end){
			createMesh();
		}},
		parser{ "usemtl ", [&](const char* begin, const char* end){
			const std::string name(begin, end + 1);
			const auto it = std::find_if(materials.begin(), materials.end(), [&name](const auto& mtl){
				return mtl.name == name;
			});
			materialIndex = it == materials.end() ? -1 : (it - materials.begin());
		}},
		parser{ "mtllib ", [&](const char* begin, const char* end){
			parseMTL(directory + std::string(begin, end + 1), directory, materials);
		}}
	);
	createMesh();
}

void MeshLoader::parseMTL(const std::string& filename, const std::string& directory, std::vector<material>& materials) {
	material* mtl = nullptr;
	
	parseFile<BUFFER_SIZE>(filename,
		parser{ "map_Kd ", [&](const char* begin, const char* end){
			mtl->textureAttribute = new meshTexture(texture::load(directory + std::string(begin, end + 1), 4));	
		}},
		parser{ "Kd ", [&](const char* begin, const char* end){
			char* next;
			glm::fvec4 c {
				std::strtof(begin, &next),
				std::strtof(++next, &next),
				std::strtof(++next, &next),
				1
			};

			if (mtl->colorAttribute) {
				mtl->colorAttribute->c = c;
			} else {
				mtl->colorAttribute = new meshColor(c);
			}	
		}},
		parser{ "d ", [&](const char* begin, const char* end){
			char* next;
			float alpha = std::strtof(begin, &next);
			if (mtl->colorAttribute) {
				mtl->colorAttribute = new meshColor(glm::fvec4{ 0, 0, 0, alpha });
			} else {
				mtl->colorAttribute->c.a = alpha;
			}	
		}},
		parser{ "newmtl ", [&](const char* begin, const char* end){
			if (mtl) materials.push_back(std::move(*mtl));
			mtl = new material(std::string(begin, end + 1));
		}}
	);

	if (mtl) materials.push_back(std::move(*mtl));
}
