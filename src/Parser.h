#ifndef _PARSER_H_
#define _PARSER_H_

// std
#include <string>
#include <vector>
#include <map>

// boost
#include <boost/variant.hpp>

// roptool
#include "AST.h"
#include "Types.h"

RopScriptShared parse(const char *name, WordLength arch_bitlen = DWORD_LENGTH);

#endif // _PARSER_H_
