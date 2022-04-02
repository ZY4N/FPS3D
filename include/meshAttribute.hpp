#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>
#include <primitives.hpp>
#include <concepts>
#include <intX.hpp>
#include <shader.hpp>

namespace mesh_attr {
	
	struct attribute {
		using vertexComp = void;
		inline virtual void preRender(shader& s) {};
		inline virtual void postRender(shader& s) {};
	};
}