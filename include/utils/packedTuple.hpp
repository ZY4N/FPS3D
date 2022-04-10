#pragma once
#include <algorithm>
#include <array>
#include <utils/intX.hpp>

template<typename... Tss>
static constexpr std::array<size_t, sizeof...(Tss)> getOffsets() {
	constexpr size_t sizes[]{sizeof(Tss)...};
	std::array<size_t, sizeof...(Tss)> offsets;
	offsets[0] = 0;
	for (size_t i = 0; i < sizeof...(Tss) - 1; i++) {
		offsets[i + 1] = offsets[i] + sizes[i]; 
	}
	return offsets;
}

template<typename... Ts>
struct packedTuple {

	u8 data[(0 + ... + sizeof(Ts))];

	constexpr packedTuple() = default;
	
	inline packedTuple(const Ts&... args) {
		copyArgs(0, args...);
	}

	template<typename T, typename... Tss>
	inline void copyArgs(u32 byteOffset, const T& first, const Tss&... args) {
		std::copy((u8*)&first, (u8*)(&first + 1), data + byteOffset);
		if constexpr (sizeof...(args) > 0) {
			copyArgs(byteOffset + sizeof(T), args...);
		}
	}

	template<size_t I, size_t J, typename T, typename... Tss>
	static inline auto& get_helper(uint8_t* data) {
		if constexpr (I == J) {
			return *(T*)data;
		} else if constexpr (sizeof...(Tss) > 0){
			return get_helper<I, J + 1, Tss...>(data + sizeof(T));
		}
	}

	template<size_t I>
	inline auto& get() {
		static_assert(I < sizeof...(Ts), "get index out of bounds");
		return get_helper<I, 0, Ts...>(data);
	}


	template<size_t I, size_t J, typename T, typename... Tss>
	static inline const auto& get_helper(const uint8_t* data) {
		if constexpr (I == J) {
			return *(T*)data;
		} else if constexpr (sizeof...(Tss) > 0){
			return get_helper<I, J + 1, Tss...>(data + sizeof(T));
		}
	}


	template<size_t I>
	inline const auto& get() const {
		static_assert(I < sizeof...(Ts), "get index out of bounds");
		return get_helper<I, 0, Ts...>(data);
	}


	template<size_t I, size_t J, typename T, typename U, typename... Us>
	static inline void set_helper(const T& src, u8* dst) {
		if constexpr (I == J) {
			static_assert(
				std::is_same<T, U>::value,
				"type at index does not match given type"
			);
			u8* srcPtr = (u8*)&src;
			std::copy(srcPtr, srcPtr + sizeof(T), dst);
		} else if constexpr (sizeof...(Us) > 0){
			return set_helper<I, J + 1, T, Us...>(dst + sizeof(T));
		}
	}

	template<size_t I, typename T>
	inline void set(const T& value) {
		static_assert(I < sizeof...(Ts), "get index out of bounds");
		return set_helper<I, 0, T, Ts...>(value, data);
	}

	static constexpr size_t sizeOf(int i) {
		constexpr int sizes[]{ sizeof(Ts)... };
		return sizes[i];
	}

	static constexpr size_t offsetOf(int i) {
		constexpr auto offsets = getOffsets<Ts...>();
		return offsets[i];
	}
};
