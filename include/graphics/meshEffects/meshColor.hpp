#pragma once

#include <utils/primitives.hpp>
#include <utils/string_literal.hpp>
#include <graphics/meshEffect.hpp>

#include <glm/gtx/string_cast.hpp>

struct meshColor {
	glm::fvec4 c;

	static constexpr char mergeVar[] = "colorMerge";
	static constexpr char colorVar[] = "meshColor";

	meshColor(const glm::fvec4& theColor) : c{ theColor }{
		//c += glm::fvec4{ 0.2, 0.2, 0.2, 0.2 }; // just for debugging
	}

	template<string_literal... Ns>
	inline void preRender(shader<Ns...>& s) const {
		s.template set<mergeVar>(1.0f);
		s.template set<colorVar>(c);
	}

	template<string_literal... Ns>
	inline void postRender(shader<Ns...>& s) const {
	}
};
