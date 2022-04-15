#pragma once

#include <geometry/meshCommon.hpp>

namespace meshes {
static shapeMesh icosahedron {
	{
		{ { 0, -0.5, 0, }, { 0.181819, 0, }, { 0.187588, -0.794647, 0.577362, } },
		{ { 0.3618, -0.223607, 0.26286, }, { 0.272728, 0.157461, }, { 0.187588, -0.794647, 0.577362, } },
		{ { -0.138192, -0.223607, 0.42532, }, { 0.09091, 0.157461, }, { 0.187588, -0.794647, 0.577362, } },
		{ { 0, -0.5, 0, }, { 0.363637, 0, }, { 0.607064, -0.794653, 0, } },
		{ { 0.3618, -0.223607, -0.26286, }, { 0.454546, 0.157461, }, { 0.607064, -0.794653, 0, } },
		{ { 0, -0.5, 0, }, { 0.909091, 0, }, { -0.491092, -0.794687, 0.356794, } },
		{ { -0.138192, -0.223607, 0.42532, }, { 1, 0.157461, }, { -0.491092, -0.794687, 0.356794, } },
		{ { -0.447212, -0.223607, 0, }, { 0.818182, 0.157461, }, { -0.491092, -0.794687, 0.356794, } },
		{ { 0, -0.5, 0, }, { 0.727273, 0, }, { -0.491092, -0.794687, -0.356794, } },
		{ { -0.138192, -0.223607, -0.42532, }, { 0.636364, 0.157461, }, { -0.491092, -0.794687, -0.356794, } },
		{ { 0, -0.5, 0, }, { 0.545455, 0, }, { 0.187588, -0.794647, -0.577362, } },
		{ { 0.447212, 0.223607, 0, }, { 0.363637, 0.314921, }, { 0.982244, -0.187608, 0, } },
		{ { 0.138192, 0.223607, 0.42532, }, { 0.181819, 0.314921, }, { 0.303495, -0.187597, 0.934183, } },
		{ { -0.3618, 0.223607, 0.26286, }, { 0.909091, 0.314921, }, { -0.79461, -0.187602, 0.577408, } },
		{ { -0.3618, 0.223607, -0.26286, }, { 0.727273, 0.314921, }, { -0.79461, -0.187602, -0.577408, } },
		{ { 0.138192, 0.223607, -0.42532, }, { 0.545455, 0.314921, }, { 0.303495, -0.187597, -0.934183, } },
		{ { -0.3618, 0.223607, 0.26286, }, { 0, 0.314921, }, { -0.303495, 0.187597, 0.934183, } },
		{ { 0, 0.5, 0, }, { 0.272728, 0.472382, }, { 0.491092, 0.794687, 0.356794, } },
		{ { 0, 0.5, 0, }, { 0.09091, 0.472382, }, { -0.187588, 0.794647, 0.577362, } },
		{ { 0, 0.5, 0, }, { 0.818182, 0.472382, }, { -0.607064, 0.794653, 0, } },
		{ { 0, 0.5, 0, }, { 0.636364, 0.472382, }, { -0.187588, 0.794647, -0.577362, } },
		{ { 0, 0.5, 0, }, { 0.454546, 0.472382, }, { 0.491092, 0.794687, -0.356794, } }
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
