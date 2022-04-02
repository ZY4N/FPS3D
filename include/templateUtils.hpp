#pragma once

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
static constexpr bool any_of() {
	return (std::is_same<T, Ts>::value || ...);
}

template<typename T, typename... Ts>
static constexpr bool containsAtIndex(auto index) {
	constexpr bool matches[]{
		std::is_same<T, Ts>::value...
	};
	return matches[index];
}
