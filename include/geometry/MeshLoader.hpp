#pragma once

#include <string>
#include <utils/intX.hpp>
#include <utils/texture.hpp>
#include <geometry/meshImpl.hpp>
#include <graphics/meshEffects.hpp>
#include <cstring>

struct material {
	std::string name; //should not be in here 
	meshColor* colorAttribute{ nullptr };
	meshTexture* textureAttribute{ nullptr };
	material(const std::string& str) : name{ str } {}
	~material() {
		delete colorAttribute;
		delete textureAttribute;
	}
	material(const material&) = delete;
	material& operator=(const material&) = delete;

	material(material&& other) :
		name{ other.name },
		colorAttribute{ other.colorAttribute },
		textureAttribute{ other.textureAttribute }
	{
		other.colorAttribute = nullptr;
		other.textureAttribute = nullptr;
	}

	material& operator=(material&& other) {	
		name = std::move(other.name);
		colorAttribute = std::move(other.colorAttribute);
		textureAttribute = std::move(other.textureAttribute);
		other.colorAttribute = nullptr;
		other.textureAttribute = nullptr;
		return *this;
	}

};

template<vertex_comp... Cs>
struct vertexID {
	u32 indices[sizeof...(Cs)]{
		(0 * sizeof(typename Cs::type))...  // init to zero
	};
	friend auto operator<=>(const vertexID&, const vertexID&) = default;
};

template<vertex_comp... Cs>
struct indexedVertexID {
	vertexID<vertex_comps::position, Cs...> id;
	u32 bufferIndex{ U32_MAX };

	indexedVertexID(u32* indices) {
		std::copy(indices, indices + (1 + ... + sizeof(Cs)), id.indices);
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
	void loadFromOBJ(const std::string& filename, std::vector<mesh<Cs...>>& destination, std::vector<material>& materials);

	void parseMTL(const std::string& filename, const std::string& directory, std::vector<material>& materials);

}
