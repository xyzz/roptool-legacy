#ifndef _ROP_SECTION_H_
#define _ROP_SECTION_H_

// roptool
#include "RopFunctionCall.h"

// std
#include <memory>

class RopSection
{
    public:
        const std::string& name(void);
        void setName(const std::string& name);
        void add(const RopFunctionCallPtr& call);
       
    private:
        std::string m_name;
        RopFunctionCallPtrList m_calls;
};

typedef std::shared_ptr<RopSection> RopSectionPtr;

#endif // _ROP_SECTION_H_
