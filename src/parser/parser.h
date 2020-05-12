#pragma once

#define SHUZIXI_DEBUG
#ifdef SHUZIXI_DEBUG
#define debug_print(x) \
    std::cout << "function " << __FUNCTION__ << " --- " << x << std::endl
#else
#define debug_print(x)
#endif // SHUZIXI_DEBUG

#define TIGER_SOURCE    "examples/test.tiger"

#include <set>
// Set for available typename
class TypeSet {
private:
    std::set<std::string> _typeset;
public:
    TypeSet() {
        _typeset.insert({ "int", "string" });
    }
    bool contains(std::string s) {
        return _typeset.find(s) != _typeset.end();
    }
    void insert(std::string s) {
        _typeset.insert(s);
    }
};

extern TypeSet typeset;

#ifndef YYSTYPE
#define YYSTYPE std::string
#endif