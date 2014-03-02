#ifndef _ROP_FUNCTION_H_
#define _ROP_FUNCTION_H_

// roptool
#include "RopFunctionCall.h"

// std
#include <memory>
#include <list>

class RopFunction
{
    public:
        const std::string& name(void);
        void setName(const std::string& name);
        void add(const RopFunctionCallPtr& call);
        std::vector<u8> binary(WordLength bits, bool little_endian);
        
    private:
        std::string m_name;
        RopFunctionCallPtrList m_calls;
};

typedef std::shared_ptr<RopFunction> RopFunctionPtr;
typedef std::list<RopFunctionPtr> RopFunctionPtrList;

#endif // _ROP_FUNCTION_H_
