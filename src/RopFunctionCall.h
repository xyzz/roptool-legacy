#ifndef _ROP_FUNCTION_CALL_H_
#define _ROP_FUNCTION_CALL_H_

// std
#include <memory>

// roptool
#include "GadgetMap.h"

class RopFunctionCall
{
    public:
        void setMap(GadgetMapPtr map);
        void setFunction(Function func);
        void setParameters(DataRefPtrList refs);
        
    private:
        GadgetMapPtr m_map;
        Function m_func;
        DataRefPtrList m_refs;
};

typedef std::shared_ptr<RopFunctionCall> RopFunctionCallPtr;

#endif _ROP_FUNCTION_CALL_H_
