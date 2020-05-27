#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <deque>
#include <unordered_map>

template <typename T>
class SymbolTable {
//deque, for different scope
	std::deque<std::unordered_map<std::string, T *>> SymbolStack;
public:
	SymbolTable();
	~SymbolTable();
	void enter();
	void exit();
	void push(std::string const &name, T *const val);
	void pop(std::string const &name);
	T *&operator[](std::string const &name);
	T *&find(std::string const &name);
	void resetTable();
};

template <typename T>
SymbolTable<T>::SymbolTable() {
	enter();
}

template <typename T>
SymbolTable<T>::~SymbolTable() {
	exit();
}

//create a new scope
template <typename T>
void SymbolTable<T>::enter() {
	SymbolStack.push_front(std::unordered_map<std::string, T *>());
}

//delete current scope
template <typename T>
void SymbolTable<T>::exit() {
	SymbolStack.pop_front();
}

//in current scope
template <typename T>
void SymbolTable<T>::push(std::string const &name, T *const val) {
	SymbolStack.front()[name] = val;
}

//in current scope
template <typename T>
void SymbolTable<T>::pop(std::string const &name) {
	SymbolStack.front().erase(name);
}

//find name in all scopes
template <typename T>
T *&SymbolTable<T>::operator[](std::string const &name) {
	for (auto stack : SymbolStack) {
		if (stack[name])
			return stack[name];
	}
	return nullptr;
}

//find name in current scope
template <typename T>
T *&SymbolTable<T>::find(std::string const &name) {
	if (SymbolStack.front()[name]) 
		return SymbolStack.front()[name];
	return nullptr;
}

template <typename T>
void SymbolTable<T>::resetTable() {
	SymbolStack.clear();
	enter();
}

#endif
