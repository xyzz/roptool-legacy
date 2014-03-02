// roptool
#include "RopSection.h"

bool RopSection::add(const RopFunctionPtr& func)
{
    // check if we have a function with this name already
    auto it = std::find_if(m_funcs.begin(), m_funcs.end(), [&](const RopFunctionPtr& func_ptr)
    {
        // check if names are equal
        return (func->name() == func_ptr->name());
    });
    
    // check if iterator is valid
    if (it != m_funcs.end())
    {
        // function already added with same name
        return false;
    }
    
    // add to list
    m_funcs.push_back(func);
    return true;
}
