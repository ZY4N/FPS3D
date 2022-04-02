#pragma once

#include <string>
#include <intX.hpp>
#include <texture.hpp>
#include <meshImpl.hpp>
#include <meshAttributes.hpp>
#include <cstring>
#include <utils.hpp>

struct material {
	std::string name;
	dynamicColor* colorAttribute{ nullptr };
	dynamicTexture* textureAttribute{ nullptr };
	material(const std::string& str) : name{ str } {}
};

template<vertex_comp... Cs>
struct vertexID {
	u32 indices[sizeof...(Cs)];
	friend auto operator<=>(const vertexID&, const vertexID&) = default;
};

template<vertex_comp... Cs>
struct indexedVertexID {
	vertexID<vertex_comps::position, Cs...> id;
	u32 bufferIndex{ (u32)-1 };

	indexedVertexID(u32* indices) {
		std::memcpy(id.indices, indices, (1 + ... + sizeof(Cs)));
	}

	friend auto operator<=>(const indexedVertexID& a, const indexedVertexID& b) {
		return a.id <=> b.id;
	}
	bool operator==(const indexedVertexID& other) const noexcept {
		return other.id == id;
	}
};

namespace MeshLoader {

	template<vertex_comp... Cs>
	void loadFromOBJ(const std::string& filename, std::vector<mesh<Cs...>>& destination);

	void parseMTL(const std::string& filename, const std::string& directory, std::vector<material>& materials);

}
