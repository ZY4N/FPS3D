#pragma once

#include <primitives.hpp>
#include <intX.hpp>

template<typename T>
concept vertex_comp = sizeof(typename T::type) > 0;

namespace vertex_comps {
struct position	{ using type = vertex3D; };
struct texCoord	{ using type = vertex2D; };
struct normal	{ using type = vertex3D; };
struct color	{ using type = color;	 };
}
