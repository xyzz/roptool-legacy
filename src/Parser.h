#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>

typedef struct
{
    std::string name;
    unsigned int number;
} FuncDecl;

bool parse(const char *name);

#endif // _PARSER_H_
