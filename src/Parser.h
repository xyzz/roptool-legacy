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

RopScriptShared parse(const char *name);

#endif // _PARSER_H_
