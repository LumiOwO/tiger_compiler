#include <iostream>
#include <cstring>

extern "C" {

void Print(char *c) { 
	std::cout << c; 
}

void Printi(std::uint64_t integer) { 
	std::cout << integer; 
}

void Flush() { 
	std::cout.flush(); 
}

char *Getchar() {
	char *input = new char[2];
	input[1] = '\0';
	if (std::cin >> input[0])
		return input;
	else {
		input[0] = '\0';
		return input;
	}
}

int Ord(char *c) {
	char tmp = *c;
	if (tmp < 0 || tmp > 127)
		return -1;
	else
		return (int)tmp;
}

char *Chr(int c) {
	if (c < 0 || c > 127) 
		exit(-1);
	return new char[2]{(char)(c), '\0'};
}

int _Size(char *c) { 
	return strlen(c); 
}

char *Substring(char *s, int f, int n) {
  char *result = new char[n + 1];
  memcpy(result, s + f, n);
  result[n] = '\0';
  return result;
}

char *Concat(char *s1, char *s2) {
	int l1 = strlen(s1), l2 = strlen(s2);
	int len = l1 + l2;
	char *result = new char[len + 1];
	memcpy(result, s1, l1);
	memcpy(result + l1, s2, l2);
	result[len] = '\0';
	return result;
}

int Not(int i) {
 	return i == 0;
}

void Exit(int i) {
 	exit(i);
}

int Strcmp(char *a, char *b) {
	return std::strcmp(a, b);
}

std::uint8_t *AllocateArray(std::uint64_t size, std::uint64_t elementSize) {
	return (std::uint8_t *)malloc(size * elementSize);
}

std::uint8_t *AllocateRecord(std::uint64_t size) {
	return (std::uint8_t *)malloc(size);
}

}