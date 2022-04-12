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


u32 countEdges(const std::vector<u16>& indices) {

	std::vector<indexedEdgeID> edgeVerticesToEdgeIndex;

	const auto addUnique = [&](u16 v1, u16 v2) {
		bool switched = v1 < v2;
		indexedEdgeID eID(switched ? v2 : v1, switched ? v1 : v2);
		const auto it = std::upper_bound(edgeVerticesToEdgeIndex.begin(), edgeVerticesToEdgeIndex.end(), eID);
		if (it == edgeVerticesToEdgeIndex.begin() || *(it - 1) != eID) {
			edgeVerticesToEdgeIndex.insert(it, eID);
		}
	};

	for (size_t i = 0; i < indices.size(); i += 3) {
		addUnique(indices[i+0], indices[i+1]);
		addUnique(indices[i+1], indices[i+2]);
		addUnique(indices[i+2], indices[i+0]);
	}
	return edgeVerticesToEdgeIndex.size();
}

namespace meshes {

// will be removed
u32 predictEdgeVertexCount(u32 depth) {
	u32 numVertices = 2;
	u32 powerOfTwo = 1;
	for (u32 i = 0; i < depth; i++) {
		numVertices += powerOfTwo;
		powerOfTwo *= 2;
	}
	return numVertices;
}

u32 predictEdgeCount(u32 depth) {
	u32 edgeCount = 2;
	u32 powerOfTwo = 1;
    for (u32 i = 1; i < depth; i++) {
        edgeCount = edgeCount * 4 - powerOfTwo;
        powerOfTwo *= 2;
	}
	return 20 * edgeCount + powerOfTwo;
}

u32 predictVertexCount(u32 depth) {
	u32 vertexCount = 22;
	u32 powerOfFour = 1;
    for (u32 i = 1; i < depth; i++) {
        vertexCount = 2 * vertexCount + (20 * powerOfFour - 1);
        powerOfFour *= 4;
	}
	return vertexCount;
}

// this will be cleaned up soon
static shapeMesh createSphere(const shapeMesh& oldSphere, u32 extraDepth) {

	const auto& oldVertices = oldSphere.getVertexBuffer();
	const auto& oldIndices = oldSphere.getIndexBuffer();

	// has to be cleaned up
	const u32 lasT = oldIndices.size() / 60;
	const u32 lastDepth = sqrt(lasT);
	const u32 thisDepth = lastDepth * (extraDepth + 1);
	const u32 thisT = thisDepth * thisDepth;
	const u32 lastNumEdges = 30 * lasT + 11 * lastDepth;			// follows 10 * n^2 + 11 * n + 1
	const u32 currentNumVertices = 10 * thisT + 11 * thisDepth + 1;	// follows 30 * n^2 + 11 * n
	const u32 currentNumIndices = 60 * thisT;						// follows 60 * n^2
	const u32 verticesPerEdge = 2 + extraDepth;
	const u32 numNewEdgeVertices = extraDepth;

	// store the new vertices for each edge for fast lookup
	u16* edgeVertexIndices = new u16[numNewEdgeVertices * lastNumEdges];

	// indexBuffer allocated in one go
	std::vector<u16> indexBuffer;
	indexBuffer.reserve(currentNumIndices);

	// vertexBuffer allocated in one go
	std::vector<shapeMesh::vertex> vertexBuffer;
	vertexBuffer.reserve(currentNumVertices);
	vertexBuffer.insert(vertexBuffer.begin(), oldVertices.begin(), oldVertices.end());
	
	// this will be the insertion index
	size_t edgeIndex = 0;
	// lookup to convert vertex pair to index (order should not matter) 
	std::vector<indexedEdgeID> edgeVerticesToEdgeIndex;
	edgeVerticesToEdgeIndex.reserve(lastNumEdges);

	u16* topIndices = new u16[numNewEdgeVertices + 1];

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

		topIndices[0] = index1; // store tip of old triangle
		size_t topPosition; // index in topIndices array

		const float verticalStepScale = 1.0 / (verticesPerEdge - 1);
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
	delete[] topIndices;

	return shapeMesh(std::move(vertexBuffer), std::move(indexBuffer));
}

}
