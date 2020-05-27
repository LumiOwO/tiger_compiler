#include <iostream>

extern "C" {

std::uint8_t *allocateRecord(std::uint64_t size) {
	return (std::uint8_t *)malloc(size);
}

std::uint8_t *allocateArray(std::uint64_t size, std::uint64_t elementSize) {
	return (std::uint8_t *)malloc(size * elementSize);
}

int _strcmp(char *a, char *b) {
	return std::strcmp(a, b);
}

}