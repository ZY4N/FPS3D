
 #include <MeshLoader.hpp>

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
//mtllib default.mtl

constexpr auto BUFFER_SIZE = 16 * 1024;

void MeshLoader::loadFromOBJ(const std::string& filename, std::vector<mesh>& destination) {

	//temporary storage for parsing
	std::vector<vertex3D> vertices, normals;
	std::vector<vertex2D> texCoords;
	std::vector<material> materials;
	
	//the final buffers for the gpu
	std::vector<u8> vertexBuffer;
	std::vector<u16> indexBuffer;

	//vertex combinations for indexing
#ifdef SORTED_IMPLEMENTATION
	std::vector<indexedVertexID> vertexIDs;
#else
	std::vector<vertexID> vertexIDs;
#endif

	bool hasTexCoords, hasNormals;
	size_t materialIndex = -1;
	size_t stride = -1;
	char parsing = -1, parsed = -1;

	const auto createMesh = [&]() {
		if (vertexBuffer.size() > 0) {
			mesh newMesh = (hasTexCoords ?
				(hasNormals ? mesh::createMesh<vertex2D, vertex3D> : mesh::createMesh<vertex2D>) :
				(hasNormals ? mesh::createMesh<vertex3D> : mesh::createMesh<>)
			)(vertexBuffer, indexBuffer);

			if (materialIndex != -1) {
				const auto& mtl = materials[materialIndex];
				if (mtl.colorAttribute && !mtl.textureAttribute)
					newMesh.addAttribute(mtl.colorAttribute);
				if (mtl.textureAttribute)
					newMesh.addAttribute(mtl.textureAttribute);
			}

			destination.push_back(std::move(newMesh));
			
			//std::cout << "found " << (vertexBuffer.size() / stride) << " vertices\n";
		}

		vertexBuffer.clear();
		indexBuffer.clear();
		vertexIDs.clear();

		materialIndex = stride = -1;
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
			// parse face structure on first face
			if (stride == -1) {
				int index = 0;
				hasTexCoords = hasNormals = false;
				for (const char* c = begin; c <= end; c++) {
					if (*c == '/') {
						index++;
					} else if (*c == ' ') {
						break;
					} else if (index == 1) {
						hasTexCoords = true;
					} else if (index == 2) {
						hasNormals = true;
					}
				}
				stride = sizeof(vertex3D) +
					(hasTexCoords ? sizeof(vertex2D) : 0) +
					(hasNormals ? sizeof(vertex3D) : 0); 
			}

			u32 firstIndex, prevIndex, compIndex = 0;

			//index for position, texCoord, normal 
			u32 indices[3]{ 0, 0, 0 };

			size_t charIndex = 0;
			const char* reader = begin;
			for (int vertexindex = 0; reader <= end + 1; ) {
				if (reader == end + 1 || *reader == ' ') {			

					u32 index;

#ifdef SORTED_IMPLEMENTATION
					indexedVertexID vID { indices[0], indices[1], indices[2], 0 };
					const auto it = std::upper_bound(vertexIDs.begin(), vertexIDs.end(), vID);

					if (it > vertexIDs.begin() && *(it - 1) == vID) {
						index = (it - 1)->bufferIndex;
					} else {
						index = vID.bufferIndex = vertexBuffer.size() / stride;
						vertexIDs.insert(it, vID);
						
						const auto push = [&vertexBuffer]<typename T>(const T& data) {
							vertexBuffer.insert(vertexBuffer.end(), (u8*)&data, (u8*)&data + sizeof(T));
						};
					
						vertices.reserve(stride);
						push(vertices[indices[0]]);
						if (hasTexCoords)
							push(texCoords[indices[1]]);
						if (hasNormals)
							push(normals[indices[2]]);
					}
#else			
					vertexID vID { indices[0], indices[1], indices[2] };
					auto it = std::find(vertexIDs.begin(), vertexIDs.end(), vID);
					if (it == vertexIDs.end()) {
						index = vertexIDs.size();
						vertexIDs.push_back(vID);

						const auto push = [&vertexBuffer]<typename T>(const T& data) {
							vertexBuffer.insert(vertexBuffer.end(), (u8*)&data, (u8*)&data + sizeof(T));
						};
						
						vertices.reserve(stride);
						push(vertices[indices[0]]);
						if (hasTexCoords)
							push(texCoords[indices[1]]);
						if (hasNormals)
							push(normals[indices[2]]);
					} else {
						index = it - vertexIDs.begin();
					}
#endif

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
					indices[compIndex] = strtol(reader, &next, 10) - 1;
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
			mtl->textureAttribute = new dynamicTexture(texture::load(directory + std::string(begin, end + 1), 4));	
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
				mtl->colorAttribute = new dynamicColor(c);
			}	
		}},
		parser{ "d ", [&](const char* begin, const char* end){
			char* next;
			float alpha = std::strtof(begin, &next);
			if (mtl->colorAttribute) {
				mtl->colorAttribute = new dynamicColor(glm::fvec4{ 0, 0, 0, alpha });
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
