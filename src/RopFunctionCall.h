#ifndef _ROP_FUNCTION_CALL_H_
#define _ROP_FUNCTION_CALL_H_

// roptool
#include "GadgetMap.h"
#include "Target.h"

// std
#include <memory>
#include <list>

class RopFunctionCall
{
    public:
        void setMap(GadgetMapPtr map);
        void setFunction(Function func);
        void setParameters(DataRefPtrList refs);
        std::vector<u8> binary(TargetPtr target);
        
    private:
        GadgetMapPtr m_map;
        Function m_func;
        DataRefPtrList m_refs;
};

typedef std::shared_ptr<RopFunctionCall> RopFunctionCallPtr;
typedef std::list<RopFunctionCallPtr> RopFunctionCallPtrList;

#endif _ROP_FUNCTION_CALL_H_
