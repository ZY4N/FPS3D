#pragma once
#include <bitset>
#include <string>
#include <stdexcept>

struct Position {
	size_t x, y;
};

struct texture {
	
	size_t dataSize;
	uint8_t* data;
	uint32_t width,height;
	uint8_t numChannels;

	texture() : width{ 0 }, height{ 0 }, numChannels{ 0 }, dataSize{ 0 }, data{ nullptr } {}

	texture(uint32_t _width, uint32_t _height, uint8_t _numChannels, uint8_t* _data) :
		width(_width), height(_height), numChannels(_numChannels), 
		dataSize((uint64_t)_width * (uint64_t)_height* (uint64_t)_numChannels), data(_data) {};

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
