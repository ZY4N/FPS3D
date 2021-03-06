#pragma once

#include <geometry/meshCommon.hpp>

namespace meshes {
static shapeMesh icosahedron {
	{
		{ { 0, -0.5, 0 }, { 0.181819, 0 }, { 0, -1, 0 } },
		{ { 0.3618, -0.223607, 0.26286 }, { 0.272728, 0.157461 }, { 0.723608, -0.447219, 0.525726 } },
		{ { -0.138192, -0.223607, 0.42532 }, { 0.09091, 0.157461 }, { -0.276387, -0.447219, 0.85065 } },
		{ { 0, -0.5, 0 }, { 0.363637, 0 }, { 0, -1, 0 } },
		{ { 0.3618, -0.223607, -0.26286 }, { 0.454546, 0.157461 }, { 0.723608, -0.447219, -0.525726 } },
		{ { 0, -0.5, 0 }, { 0.909091, 0 }, { 0, -1, 0 } },
		{ { -0.138192, -0.223607, 0.42532 }, { 1, 0.157461 }, { -0.276387, -0.447219, 0.85065 } },
		{ { -0.447212, -0.223607, 0 }, { 0.818182, 0.157461 }, { -0.894426, -0.447215, 0 } },
		{ { 0, -0.5, 0 }, { 0.727273, 0 }, { 0, -1, 0 } },
		{ { -0.138192, -0.223607, -0.42532 }, { 0.636364, 0.157461 }, { -0.276387, -0.447219, -0.85065 } },
		{ { 0, -0.5, 0 }, { 0.545455, 0 }, { 0, -1, 0 } },
		{ { 0.447212, 0.223607, 0 }, { 0.363637, 0.314921 }, { 0.894426, 0.447215, 0 } },
		{ { 0.138192, 0.223607, 0.42532 }, { 0.181819, 0.314921 }, { 0.276387, 0.447219, 0.85065 } },
		{ { -0.3618, 0.223607, 0.26286 }, { 0.909091, 0.314921 }, { -0.723608, 0.447219, 0.525726 } },
		{ { -0.3618, 0.223607, -0.26286 }, { 0.727273, 0.314921 }, { -0.723608, 0.447219, -0.525726 } },
		{ { 0.138192, 0.223607, -0.42532 }, { 0.545455, 0.314921 }, { 0.276387, 0.447219, -0.85065 } },
		{ { -0.3618, 0.223607, 0.26286 }, { 0, 0.314921 }, { -0.723608, 0.447219, 0.525726 } },
		{ { 0, 0.5, 0 }, { 0.272728, 0.472382 }, { 0, 1, 0 } },
		{ { 0, 0.5, 0 }, { 0.09091, 0.472382 }, { 0, 1, 0 } },
		{ { 0, 0.5, 0 }, { 0.818182, 0.472382 }, { 0, 1, 0 } },
		{ { 0, 0.5, 0 }, { 0.636364, 0.472382 }, { 0, 1, 0 } },
		{ { 0, 0.5, 0 }, { 0.454546, 0.472382 }, { 0, 1, 0 } }
	},
	{
		0, 1, 2,
		1, 3, 4,
		5, 6, 7,
		8, 7, 9,
		10, 9, 4,
		1, 4, 11,
		2, 1, 12,
		7, 6, 13,
		9, 7, 14,
		4, 9, 15,
		1, 11, 12,
		2, 12, 16,
		7, 13, 14,
		9, 14, 15,
		4, 15, 11,
		12, 11, 17,
		16, 12, 18,
		14, 13, 19,
		15, 14, 20,
		11, 15, 21
	}
};
}
