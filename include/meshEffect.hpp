#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>
#include <primitives.hpp>
#include <concepts>
#include <intX.hpp>
#include <shader.hpp>

struct meshEffect {
	inline virtual void preRender(shader& s) {};
	inline virtual void postRender(shader& s) {};
};
