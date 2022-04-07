#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>
#include <primitives.hpp>
#include <concepts>
#include <intX.hpp>
#include <shader.hpp>

//Ns represent the names of unfiorms in the shader the effect makes use of
template<const char*... Ns>
struct meshEffect {
	//inline virtual void preRender(shader<Ns...>& s) {};
	//inline virtual void postRender(shader<Ns...>& s) {};
};
