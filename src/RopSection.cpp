// roptool
#include "RopSection.h"

const std::string& RopSection::name(void)
{
    return m_name;
}

void RopSection::setName(const std::string& name)
{
    m_name = name;
}

void RopSection::add(const RopFunctionCallPtr& call)
{
    m_calls.push_back(call);
}
