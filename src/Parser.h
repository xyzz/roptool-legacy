#ifndef _PARSER_H_
#define _PARSER_H_

// std
#include <string>
#include <vector>
#include <map>

// boost
#include <boost/variant.hpp>

typedef std::map<std::string, unsigned int> func_map;
typedef std::map<std::string, boost::variant<unsigned int, std::string>> symbol_map;

typedef std::pair<std::string, unsigned int> func_pair;
typedef std::pair<std::string, boost::variant<unsigned int, std::string>> symbol_pair;

typedef struct
{
    func_map functions;
    symbol_map symbols;
} DataDecl;

typedef boost::variant<unsigned int, std::string> parameter;

typedef struct
{
    std::string callee;
    std::vector<parameter> parameter_list;
} CallDecl;

typedef struct
{
    std::string name;
    std::vector<CallDecl> call_list;
} CodeDecl;

typedef struct
{
    DataDecl data;
    CodeDecl code;
} RopScript;

typedef std::shared_ptr<RopScript> RopScriptShared;
RopScriptShared parse(const char *name);

#endif // _PARSER_H_
