#ifndef _ROP_SECTION_H_
#define _ROP_SECTION_H_

// roptool
#include "RopFunction.h"

// std
#include <memory>

class RopSection
{
    public:
        bool add(const RopFunctionPtr& func);
        RopFunctionPtrList functions(void);
        
    private:
        RopFunctionPtrList m_funcs;
};

typedef std::shared_ptr<RopSection> RopSectionPtr;

#endif // _ROP_SECTION_H_
