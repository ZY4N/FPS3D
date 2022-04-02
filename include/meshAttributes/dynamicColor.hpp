#pragma once

#include <primitives.hpp>
#include <meshAttribute.hpp>

#include <glm/gtx/string_cast.hpp>

using namespace mesh_attr;

struct dynamicColor : public attribute {
	glm::fvec4 c;

	dynamicColor(const glm::fvec4& theColor) : c{ theColor }{
		//std::cout << glm::to_string(c) << std::endl;
		c += glm::fvec4{ 0.2, 0.2, 0.2, 0.2 };
	} 

	inline void preRender(shader& s) override {
		//s.set("colorMerge", 1.0f);
		//s.set("meshColor", c);
	}
	inline void postRender(shader& s) override {
		//s.set("colorMerge", 0.0f);
	}
};