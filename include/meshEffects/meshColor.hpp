#pragma once

#include <primitives.hpp>
#include <meshEffect.hpp>

#include <glm/gtx/string_cast.hpp>


struct meshColor : public meshEffect {
	glm::fvec4 c;

	meshColor(const glm::fvec4& theColor) : c{ theColor }{
		c += glm::fvec4{ 0.2, 0.2, 0.2, 0.2 }; // just for debugging
	} 

	inline void preRender(shader& s) override {
		s.set("colorMerge", 1.0f);
		s.set("meshColor", c);
	}
	inline void postRender(shader& s) override {
		s.set("colorMerge", 0.0f);
	}
};
