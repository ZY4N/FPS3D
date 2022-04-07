#pragma once

#include <cstring>
#include <string_literal.hpp>

template <auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F&& f) {
    if constexpr (Start < End) {
        f(std::integral_constant<decltype(Start), Start>());
        constexpr_for<Start + Inc, End, Inc>(f);
    }
}

template <class F, class... Args>
constexpr void constexpr_for(F&& f, Args&&... args) {
    (f(std::forward<Args>(args)), ...);
}

template <auto I, class F, class Arg, class... Args>
constexpr void constexpr_indexed_for(F&& f, Arg&& arg, Args&&... args) {
    f(std::integral_constant<decltype(I), I>(), std::forward<Arg>(arg));
	if constexpr (sizeof...(args) > 0) {
		constexpr_indexed_for<I + 1, F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
	}
}

template<typename T, typename... Ts>
constexpr bool any_of() {
	return (std::is_same<T, Ts>::value || ...);
}

template<typename T, typename... Ts>
constexpr bool containsAtIndex(auto index) {
	constexpr bool matches[]{
		std::is_same<T, Ts>::value...
	};
	return matches[index];
}

/*
constexpr bool constexpr_strcmp(const char* str1, const char* str2) {
	while (str1 && str2) {
		if (*str1 != *str2) {
			return false;
		}
		str1++;
		str2++;
	}
	return !str1 && !str2;
}
*/

template<string_literal STR, string_literal... STRs>
constexpr size_t string_index() {
	constexpr bool same_at_index[]{ (STR == STRs)... };
	for (size_t i = 0; i < sizeof...(STRs); i++) {
		if (same_at_index[i]) {
			return i;
		}
	}	
	return -1;
}


template<string_literal STR, string_literal... STRs>
constexpr bool contains_string() {
	return ((STR == STRs) || ...);
}


template<size_t I, size_t N,string_literal STR, string_literal... STRs>
constexpr bool contains_strings_helper() {
	if constexpr (I == N) {
		return contains_string<STR, STRs...>();
	} else {
		return contains_strings_helper<I + 1, N, STR, STRs...>();
	}
}

template<size_t N, string_literal STR, string_literal... STRs>
constexpr bool contains_strings() {
	//static_assert(N <= sizeof...(STRs) + 1, "Not enough strings given");
	if constexpr (!contains_strings_helper<0, N, STRs...>()) {
		return false;
	} else if constexpr (sizeof...(STRs) > 0) {
		return contains_strings<N - 1, STRs...>();
	} else {
		return true;
	}
}

template<size_t N, string_literal... STRs>
struct containsStrings {
	static constexpr bool value = contains_strings<N, STRs...>();
};
