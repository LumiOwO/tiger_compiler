#include <iostream>
#include <cstring>

extern "C" {

//Standard Library

//function print(s : string)
//Print the string on the standard output
void Print(char *s) { 
	std::cout << s; 
}

//function printi(i : int)
//Print the integer on the standard output
void Printi(std::uint64_t integer) { 
	std::cout << integer; 
}

//function flush()
//Flush the standard output buffer
void Flush() { 
	std::cout.flush(); 
}

//function getchar() : string
//Read and return a character from standard input; return an empty string at end-of-file.
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

//function ord(s : string) : int
//Return the ASCII value of the first character of s, or −1 if s is empty
int Ord(char *c) {
	char tmp = *c;
	if (tmp < 0 || tmp > 127)
		return -1;
	else
		return (int)tmp;
}

//function chr(i : int) : string
//Return a single-character string for ASCII value i. Terminate program if i is out of range
char *Chr(int i) {
	if (i < 0 || i > 127) 
		exit(-1);
	char *result = new char[2];
	result[0] = (char)(i);
	result[1] = '\0';
	return result;
}

//function size(s : string) : int
//Return the number of characters in s.
int _Size(char *s) { 
	return strlen(s); 
}

//function substring(s:string,f:int,n:int):string
//Return the substring of s starting at the character f (first char-acter is numbered zero) and going for n characters.
char *Substring(char *s, int f, int n) {
  char *result = new char[n + 1];
  memcpy(result, s + f, n);
  result[n] = '\0';
  return result;
}

//function concat (s1:string, s2:string):string
//Return a new string consisting of s1 followed by s2
char *Concat(char *s1, char *s2) {
	int l1 = strlen(s1), l2 = strlen(s2);
	int len = l1 + l2;
	char *result = new char[len + 1];
	memcpy(result, s1, l1);
	memcpy(result + l1, s2, l2);
	result[len] = '\0';
	return result;
}

//function not(i : int) : int
//Return 1 if i is zero, 0 otherwise.
int Not(int i) {
 	return i == 0;
}

//function exit(i : int)
//Terminate execution of the program with code i.
void Exit(int i) {
 	exit(i);
}

//execute strcmp of string a and string b
int Strcmp(char *a, char *b) {
	return std::strcmp(a, b);
}

//allocation for array type with length = size, element size = elementSize
std::uint8_t *AllocateArray(std::uint64_t size, std::uint64_t elementSize) {
	return (std::uint8_t *)malloc(size * elementSize);
}

//allocation for record type with size
std::uint8_t *AllocateRecord(std::uint64_t size) {
	return (std::uint8_t *)malloc(size);
}

}