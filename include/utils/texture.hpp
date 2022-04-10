#pragma once
#include <bitset>
#include <string>
#include <stdexcept>

struct Position {
	size_t x, y;
};

struct texture {
	
	uint8_t* data{ nullptr };
	size_t dataSize{ 0 };
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	uint8_t numChannels { 0 };

	texture() = default;

	texture(uint8_t* data, uint32_t width, uint32_t height, uint8_t numChannels) :
		data{ data }, dataSize{ (size_t)width * (size_t)height * (uint64_t)numChannels },
		width{ width }, height{ height }, numChannels{ numChannels } {};

	texture(const texture&);

	texture(texture&&) noexcept;

	~texture();

	texture& operator=(const texture&);

	texture& operator=(texture&&) noexcept;

	static texture load(const std::string& filename, uint8_t forceChannels = 0);

	static texture create(const uint32_t width, const uint32_t height, const uint8_t numChannels, const uint8_t* fill);

	static texture createAtlas(texture* textures, size_t numTextures, Position** positions);

	int save(const std::string& filename) const;

	uint32_t operator()(double x, double y) const;

	uint8_t* pixels(size_t& pixelLen);

	void set(uint32_t posX, uint32_t posY, uint8_t numChannels, const texture& img);

	uint32_t averageColor() const;
};
