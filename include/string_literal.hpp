#pragma once

#include <intX.hpp>

template<size_t N>
struct string_literal {
	constexpr string_literal(const char (&str)[N]) {
		std::copy_n(str, N, value);
	}

	template<size_t I, size_t J>
	constexpr friend bool operator==(
		const string_literal<I>& str1,
		const string_literal<J>& str2
	);
    
    char value[N];
};

template<size_t I, size_t J>
constexpr bool operator==(
	const string_literal<I>& str1,
	const string_literal<J>& str2
) {
	if constexpr (I != J) {
		return false;
	} else {
		for (size_t i = 0; i < I; i++) {
			if (str1.value[i] != str2.value[i]) {
				return false;
			}
		}
		return true;
	}
}
