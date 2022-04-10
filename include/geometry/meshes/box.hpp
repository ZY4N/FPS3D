#pragma once

#include <geometry/meshCommon.hpp>

using shapeMesh = mesh<vertex_comps::texCoord, vertex_comps::normal>;

static constexpr float third = 1/3.0f;
static constexpr float twoThirds = 2/3.0f;

namespace meshes {

static shapeMesh box {
	{
		{ {  0.5, -0.5,  0.5 }, { 1			, third		 }, { 0 , -1,  0 } },
		{ { -0.5, -0.5,  0.5 }, { 1			, twoThirds	 }, {  0, -1,  0 } },
		{ { -0.5, -0.5, -0.5 }, { twoThirds	, twoThirds	 }, {  0, -1,  0 } },
		{ { -0.5,  0.5, -0.5 }, { 1			, third		 }, {  0,  1,  0 } },
		{ { -0.5,  0.5,  0.5 }, { twoThirds	, third		 }, {  0,  1,  0 } },
		{ {  0.5,  0.5,  0.5 }, { twoThirds	, 0			 }, {  0,  1,  0 } },
		{ {  0.5,  0.5, -0.5 }, { 0			, third		 }, {  1,  0,  0 } },
		{ {  0.5,  0.5,  0.5 }, { 0			, 0			 }, {  1,  0,  0 } },
		{ {  0.5, -0.5,  0.5 }, { third		, 0			 }, {  1,  0,  0 } },
		{ {  0.5,  0.5,  0.5 }, { third		, 0			 }, { -0,  0,  1 } },
		{ { -0.5,  0.5,  0.5 }, { twoThirds	, 0			 }, { -0,  0,  1 } },
		{ { -0.5, -0.5,  0.5 }, { twoThirds	, third		 }, { -0,  0,  1 } },
		{ { -0.5, -0.5,  0.5 }, { third		, 1			 }, { -1, -0, -0 } },
		{ { -0.5,  0.5,  0.5 }, { 0			, 1			 }, { -1, -0, -0 } },
		{ { -0.5,  0.5, -0.5 }, { 0			, twoThirds	 }, { -1, -0, -0 } },
		{ {  0.5, -0.5, -0.5 }, { third		, third		 }, {  0,  0, -1 } },
		{ { -0.5, -0.5, -0.5 }, { third		, twoThirds	 }, {  0,  0, -1 } },
		{ { -0.5,  0.5, -0.5 }, { 0			, twoThirds	 }, {  0,  0, -1 } },
		{ {  0.5, -0.5, -0.5 }, { twoThirds	, third		 }, {  0, -1,  0 } },
		{ {  0.5,  0.5, -0.5 }, { 1			, 0			 }, {  0,  1,  0 } },
		{ {  0.5, -0.5, -0.5 }, { third		, third		 }, {  1,  0,  0 } },
		{ {  0.5, -0.5,  0.5 }, { third		, third		 }, { -0,  0,  1 } },
		{ { -0.5, -0.5, -0.5 }, { third		, twoThirds	 }, { -1,  0, -0 } },
		{ {  0.5,  0.5, -0.5 }, { 0			, third		 }, {  0,  0, -1 } }
	},
	{
		 0,  1,  2,
		 3,  4,  5,
		 6,  7,  8,
		 9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18,  0,  2,
		19,  3,  5,
		20,  6,  8,
		21,  9, 11,
		22, 12, 14,
		23, 15, 17
	}
};
}
