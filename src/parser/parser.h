#pragma once

#include <AST/AST.h>

// Redefine yylval type
typedef AST* ptrAST;
#ifndef YYSTYPE
#define YYSTYPE ptrAST
#endif

#include <set>

// Namespace to access yacc
namespace TigerParser {

	// Set for typename
	extern std::set<std::string> _typeset;
	// Utils for type-id
	inline bool hasType(std::string s) { return _typeset.find(s) != _typeset.end(); }
	inline void insert(std::string s) { _typeset.insert(s); }

	// string constant parser
	// catched: "a \' b"
	// actual string: a ' b
	// visualize string: \"a \\\' b\"
	extern std::ostringstream _str_real;
	extern std::ostringstream _str_label;
	// Utils for string constant
	inline void resetStrBuffer() { _str_real.str(""); _str_label.str(""); }
	inline std::string realstr() { return _str_real.str(); }
	inline std::string labelstr() { return _str_label.str(); }
	template<class T> inline void appendReal(T val) { _str_real << val; }
	template<class T> inline void appendLabel(T val) { _str_label << val; }

	// Public interfaces
	inline void reset() {
		_typeset = std::set<std::string>({ "int", "string" });
		resetStrBuffer();
	}
	AST* parse(const char *filepath);     // return error code, implement in tiger.ypp
	inline AST* parse(std::string s) {
		return parse(s.c_str());
	}

};
