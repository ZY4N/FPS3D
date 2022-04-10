#pragma once

#include <geometry/meshCommon.hpp>
#include <utils/intX.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>

struct edgeID {
	u16 vertex1{ U16_MAX };
	u16 vertex2{ U16_MAX };
	friend auto operator<=>(const edgeID&, const edgeID&) = default;
};

struct indexedEdgeID {
	edgeID id;
	u16 bufferIndex{ U16_MAX };

	indexedEdgeID() = default;

	indexedEdgeID(u16 vertex1, u16 vertex2) : id{ vertex1, vertex2 } {}

	friend auto operator<=>(const indexedEdgeID& a, const indexedEdgeID& b) {
		return a.id <=> b.id;
	}
	bool operator==(const indexedEdgeID& other) const noexcept {
		return other.id == id;
	}
};


namespace meshes {

// will be removed
template<u32 I>
constexpr u32 numEdgeVertices() {
	u32 numVertices = 2;
	u32 powerOfTwo = 1;
	for (u32 i = 0; i < I; i++) {
		numVertices += powerOfTwo;
		powerOfTwo *= 2;
	}
	return numVertices;
}

// this will be cleaned up soon
template<u32 I>
static shapeMesh createSphere(const shapeMesh& oldSphere) {

	const auto& oldVertices = oldSphere.getVertexBuffer();
	const auto& oldIndices = oldSphere.getIndexBuffer();

	// has to be cleaned up
	const size_t currentT = 1;//(oldVertices.size() - 2) / 10;
	const size_t currentNumEdges = 41;
	constexpr u32 verticesPerEdge = numEdgeVertices<I>();
	constexpr size_t numNewEdgeVertices = verticesPerEdge - 2;

	// store the new vertices for each edge for fast lookup
	u16* edgeVertexIndices = new u16[numNewEdgeVertices * currentNumEdges];

	// indexBuffer allocated in one go
	constexpr u32 triangleMultiplier = std::pow(4, I);
	std::vector<u16> indexBuffer;
	indexBuffer.reserve(oldIndices.size() * triangleMultiplier);

	// vertexBuffer allocated in one go
	const size_t lastA = sqrt(currentT);
	const size_t nextT = (lastA + 1) * (lastA + 1);
	std::vector<shapeMesh::vertex> vertexBuffer;
	vertexBuffer.reserve(10 * nextT + 2);
	vertexBuffer.insert(vertexBuffer.begin(), oldVertices.begin(), oldVertices.end());
	
	// this will be the insertion top
	size_t edgeIndex = 0;
	// lookup to convert vertex pair to index (order should not matter) 
	std::vector<indexedEdgeID> edgeVerticesToEdgeIndex;
	edgeVerticesToEdgeIndex.reserve(currentNumEdges);

	const auto findOrReserveEdgeIndex = [&](u16 v1, u16 v2, u16& index, bool& switched) {
		// swapping to unify different vertex orders
		switched = v1 < v2;
		indexedEdgeID eID(switched ? v2 : v1, switched ? v1 : v2);
		const auto it = std::upper_bound(edgeVerticesToEdgeIndex.begin(), edgeVerticesToEdgeIndex.end(), eID);

		if (it > edgeVerticesToEdgeIndex.begin() && *(it - 1) == eID) {
			index = (it - 1)->bufferIndex;
			return true;
		} else {
			index = eID.bufferIndex = edgeIndex++;
			edgeVerticesToEdgeIndex.insert(it, eID);
			return false;
		}
	};

	for (size_t i = 0; i < oldIndices.size(); i += 3) {
		
		const u16 index1 = oldIndices[i + 0];
		const u16 index2 = oldIndices[i + 1];
		const u16 index3 = oldIndices[i + 2];

		const auto& top = vertexBuffer[index1];
		const auto& right = vertexBuffer[index2];
		const auto& left = vertexBuffer[index3];

		//positions (and normals)
		const auto topPos = top.get<0>();
		const auto rightPos = right.get<0>();
		const auto leftPos = left.get<0>();

		//tex coords
		const auto topTex = top.get<1>();
		const auto rightTex = right.get<1>();
		const auto leftTex = left.get<1>();

		u16 leftEdgeIndex, rightEdgeIndex, bottomEdgeIndex;
		bool leftEdgeFlipped, rightEdgeFlipped, bottomEdgeFlipped;
		
		bool leftEdgeCached = findOrReserveEdgeIndex(index1, index3, leftEdgeIndex, leftEdgeFlipped);
		bool rightEdgeCached = findOrReserveEdgeIndex(index1, index2, rightEdgeIndex, rightEdgeFlipped);
		bool bottomEdgeCached = findOrReserveEdgeIndex(index3, index2, bottomEdgeIndex, bottomEdgeFlipped);
		
		u16* leftEdgeBuffer = &edgeVertexIndices[leftEdgeIndex * numNewEdgeVertices];
		u16* rightEdgeBuffer = &edgeVertexIndices[rightEdgeIndex * numNewEdgeVertices];
		u16* bottomEdgeBuffer = &edgeVertexIndices[bottomEdgeIndex * numNewEdgeVertices];

		u16 topIndices[numNewEdgeVertices + 1];
		topIndices[0] = index1; // store tip of old triangle
		size_t topPosition; // index in topIndices array

		constexpr float verticalStepScale = 1.0 / (verticesPerEdge - 1);
		for (u32 i = 0; i <= numNewEdgeVertices; i++) {
			float a = (i+1) * verticalStepScale;
			const auto posA = glm::normalize(glm::mix(topPos, leftPos, a)) * 0.5f;
			const auto posB = glm::normalize(glm::mix(topPos, rightPos, a)) * 0.5f;

			const auto texA = glm::mix(topTex, leftTex, a);
			const auto texB = glm::mix(topTex, rightTex, a);

			topPosition = 0;

			u16 bottomLeftIndex;
			if (i == numNewEdgeVertices) {
				bottomLeftIndex = index3;
			} else if (leftEdgeCached) {
				bottomLeftIndex = leftEdgeBuffer[leftEdgeFlipped ? numNewEdgeVertices - 1 - i : i];
			} else {
				leftEdgeBuffer[leftEdgeFlipped ? numNewEdgeVertices - 1 - i : i] = bottomLeftIndex = vertexBuffer.size();
				vertexBuffer.emplace_back(posA, texA, posA);
			}

			// creates vertex strips for each triangle row
			float horizontalStepScale = 1.0 / (i+1);
			for (u32 j = 0; j < i; j++) {
				float b = (j+1) * horizontalStepScale;
				u16 newIndex;

				if (i == numNewEdgeVertices && bottomEdgeCached) {
					newIndex = bottomEdgeBuffer[bottomEdgeFlipped ? numNewEdgeVertices - 1 - j : j];
				} else {
					bottomEdgeBuffer[bottomEdgeFlipped ? numNewEdgeVertices - 1 - j : j] = newIndex = vertexBuffer.size();
					const auto newPos = glm::normalize(glm::mix(posA, posB, b)) * 0.5f;
					const auto newTex = glm::mix(texA, texB, b);
					vertexBuffer.emplace_back(newPos, newTex, newPos);
				}

				// left triangle
				indexBuffer.push_back(topIndices[topPosition]);
				indexBuffer.push_back(newIndex);
				indexBuffer.push_back(bottomLeftIndex);

				// upside down triangle
				indexBuffer.push_back(topIndices[topPosition]);
				indexBuffer.push_back(topIndices[++topPosition]);
				indexBuffer.push_back(newIndex);

				topIndices[topPosition-1] = bottomLeftIndex;
				bottomLeftIndex = newIndex;
			}

			u16 bottomRightIndex;
			if (i == numNewEdgeVertices) {
				bottomRightIndex = index2;
			} else if (rightEdgeCached) {
				bottomRightIndex = rightEdgeBuffer[rightEdgeFlipped ? (numNewEdgeVertices - 1 - i) : i];
			} else {
				rightEdgeBuffer[rightEdgeFlipped ? numNewEdgeVertices - 1 - i : i] = bottomRightIndex = vertexBuffer.size();
				vertexBuffer.emplace_back(posB, texB, posB);		
			}

			indexBuffer.push_back(topIndices[topPosition]);
			indexBuffer.push_back(bottomRightIndex);
			indexBuffer.push_back(bottomLeftIndex);
			
			topIndices[topPosition] = bottomLeftIndex;
			if (i < numNewEdgeVertices) {
				topIndices[topPosition+1] = bottomRightIndex;
			}
		}
	}

	delete[] edgeVertexIndices;
	
	return shapeMesh(std::move(vertexBuffer), std::move(indexBuffer));
}
}
