#pragma once

#include <GL/glew.h> 
#include <SFML/OpenGL.hpp>
#include <utils/primitives.hpp>
#include <utils/intX.hpp>
#include <graphics/shader.hpp>

#include <concepts>

//Ns represent the names of unfiorms in the shader the effect makes use of
template<const char*... Ns>
struct meshEffect {
	//inline virtual void preRender(shader<Ns...>& s) {};
	//inline virtual void postRender(shader<Ns...>& s) {};
};
