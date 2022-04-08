#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include <utils/intX.hpp>
#include <utils/templateUtils.hpp>


template<typename F>
struct parser {
	const std::string_view phrase;
	F function;
};

template<size_t BUFFER_SIZE, typename... Fs>
void parseFile(const std::string& filename, parser<Fs>... parsers) {
	int fd = open(filename.c_str(), O_RDONLY);
	if (fd == -1) {
		puts(filename.c_str());
		throw "failed to open file";
	}

	posix_fadvise(fd, 0, 0, 1);

	char buffer[BUFFER_SIZE];
	size_t leftOverBytes = 0;
	int predictedIndex = 0, lastIndex = 0;

	size_t newBytes;
	while ((newBytes = read(fd, buffer + leftOverBytes, BUFFER_SIZE - leftOverBytes)) || leftOverBytes) {
		if (newBytes == -1)
			throw "failed to read file";

		bool flush = !newBytes;
		newBytes += leftOverBytes;
		
		char *lineEnd, *line = buffer;
		while (lineEnd = flush ? (buffer + leftOverBytes) : (char*)std::memchr(line, '\n', buffer + newBytes - line)) {
			bool foundMatch = false;
			do {
				constexpr_indexed_for<0>([&](const auto index, auto const& parser) {
					if (index == predictedIndex) {
						if (strncmp(parser.phrase.data(), line, parser.phrase.length()) == 0) {
							parser.function(line + parser.phrase.length(), lineEnd - 1);
							foundMatch = true;
							lastIndex = index;
						} else {
							predictedIndex = (index + 1 == sizeof...(Fs)) ? 0 : (index + 1);
							foundMatch |= (predictedIndex == lastIndex);
						}
					}
				}, parsers...);
			} while(!foundMatch);
			line = lineEnd + 1;
			if (flush) break;
		}

		leftOverBytes = flush ? 0 : buffer + newBytes - line;
		std::memmove(buffer, line, leftOverBytes);
	}
}
