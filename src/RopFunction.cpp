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

std::vector<u8> RopFunction::binary(WordLength bits, bool little_endian)
{
    std::vector<u8> function_bin;
    
    // loop through every function call
    std::for_each(m_calls.begin(), m_calls.end(), [&](const RopFunctionCallPtr& call)
    {
        // get the binary blob
        std::vector<u8> call_bin = call->binary(bits, little_endian);
        function_bin.insert(function_bin.end(), call_bin.begin(), call_bin.end());
    });
    
    return function_bin;
}