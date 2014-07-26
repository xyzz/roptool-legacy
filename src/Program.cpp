#include "Program.h"


DataSection& Program::data(void)
{
    return m_data_section;
}

RopSection& Program::code(void)
{
    return m_rop_section;
}
