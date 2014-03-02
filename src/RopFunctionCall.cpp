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

std::vector<u8> RopFunctionCall::binary(TargetPtr target)
{
    std::vector<u8> output;
    
    // set map parameters
    m_map->setFunction(m_func);
    m_map->setParameters(m_refs);
    
    // get the stack data
    std::vector<u64> stack = m_map->stack();
    
    // \todo: automatically obtain this
    bool little_endian = true;
    
    // loop through the stack and convert it to what we need
    for (unsigned int i = 0; i < stack.size(); i++)
    {
        u64 stack_val = stack.at(i);
        
        // write endianness
        for (int k = 0; k < target->manifest()->arch_bitlen(); k += 8)
        {
            // is little endian?
            if (little_endian)
            {
                // lsb first
                output.push_back((stack_val >> k) & 0xFF);
            }
            else
            {
                // msb fist
                output.push_back((stack_val >> (target->manifest()->arch_bitlen() - k - 8)) & 0xFF);
            }
        }
    }
    
    // return the binary representation of this gadget
    return output;
}
