// roptool
#include "RopFunctionCall.h"


void RopFunctionCall::setMap(GadgetMapPtr map)
{
    m_map = map;
}

void RopFunctionCall::setFunction(Function func)
{
    m_func = func;
}

void RopFunctionCall::setParameters(DataRefPtrList refs)
{
    m_refs = refs;
}
