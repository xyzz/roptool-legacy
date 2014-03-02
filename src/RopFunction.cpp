// roptool
#include "RopFunction.h"

const std::string& RopFunction::name(void)
{
    return m_name;
}

void RopFunction::setName(const std::string& name)
{
    m_name = name;
}

void RopFunction::add(const RopFunctionCallPtr& call)
{
    m_calls.push_back(call);
}
