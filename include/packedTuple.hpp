#pragma once
#include <cstring>
#include <array>

template<typename... Tss>
static constexpr std::array<int, sizeof...(Tss)> getOffsets() {
	constexpr int sizes[]{sizeof(Tss)...};
	std::array<int, sizeof...(Tss)> offsets;
	offsets[0] = 0;
	for (int i = 0; i < sizeof...(Tss) - 1; i++) {
		offsets[i + 1] = offsets[i] + sizes[i]; 
	}
	return offsets;
}

template<typename... Ts>
struct packedTuple {

	u8 data[(0 + ... + sizeof(Ts))];

	packedTuple() = default;
	
	packedTuple(Ts... args) {
		copyArgs(0, args...);
	}

	template<typename T, typename... Tss>
	inline void copyArgs(u32 byteOffset, T first, Tss... args) {
		std::memcpy(data + byteOffset, (u8*)&first, sizeof(T));
		if constexpr (sizeof...(args) > 0) {
			copyArgs(byteOffset + sizeof(T), args...);
		}
	}

	static constexpr int offsetOf(int i) const {
		constexpr auto offsets = getOffsets<Ts...>();
		return offsets[i];
	}
};
