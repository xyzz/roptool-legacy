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

std::vector<u8> RopFunction::binary(TargetPtr target)
{
    std::vector<u8> function_bin;
    
    // loop through every function call
    std::for_each(m_calls.begin(), m_calls.end(), [&](const RopFunctionCallPtr& call)
    {
        // get the binary blob
        std::vector<u8> call_bin = call->binary(target);
        function_bin.insert(function_bin.end(), call_bin.begin(), call_bin.end());
        
        // add our caller gadget
        GadgetPtr caller = target->getCallerGadget();
        
        // get the endianness 
        bool little_endian = true;
        
        while (function_bin.size() % target->manifest()->stack_alignment())
        {
            // write endianness
            for (int k = 0; k < target->manifest()->arch_bitlen(); k += 8)
            {
                // is little endian?
                if (little_endian)
                {
                    // lsb first
                    function_bin.push_back((caller->address() >> k) & 0xFF);
                }
                else
                {
                    // msb fist
                    function_bin.push_back((caller->address() >> (target->manifest()->arch_bitlen() - k - 8)) & 0xFF);
                }
            }
        }
    });
    
    return function_bin;
}