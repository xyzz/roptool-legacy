#ifndef _PROGRAM_H_
#define _PROGRAM_H_

// roptool
#include "DataSection.h"
#include "RopSection.h"

// std
#include <memory>

class Program
{
    public:
        
        // getters
        DataSection& data(void);
        RopSection& code(void);
        
    private:
        DataSection m_data_section;
        RopSection m_rop_section;
};

typedef std::shared_ptr<Program> ProgramPtr;

#endif // _PROGRAM_H_
