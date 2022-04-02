#pragma once

#include <string>
#include <intX.hpp>
#include <texture.hpp>
#include <mesh.hpp>
#include <meshAttributes.hpp>
#include <cstring>
#include <utils.hpp>

#define SORTED_IMPLEMENTATION

struct material {
	std::string name;
	dynamicColor* colorAttribute{ nullptr };
	dynamicTexture* textureAttribute{ nullptr };
	material(const std::string& str) : name{ str } {}
};

struct vertexID {
	u32 positionIndex, texCoordIndex, normalIndex;
	friend auto operator<=>(const vertexID&, const vertexID&) = default;
};

#ifdef SORTED_IMPLEMENTATION
struct indexedVertexID {
	vertexID indices;
	u32 bufferIndex;
	friend auto operator<=>(const indexedVertexID& a, const indexedVertexID& b) {
		return a.indices <=> b.indices;
	}
	bool operator==(const indexedVertexID& other) const noexcept {
		return other.indices == indices;
	}
};
#endif


namespace MeshLoader {

	void loadFromOBJ(const std::string& filename, std::vector<mesh>& destination);

	void parseMTL(const std::string& filename, const std::string& directory, std::vector<material>& materials);

}
