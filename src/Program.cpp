#include "Program.h"

std::ostream& operator<<(std::ostream& stream, const RopFileHeader& header)
{
    stream  << header.magic << header.version << header.reserved1 << header.reserved2
            << header.dsize << header.daddr << header.csize << header.centry;
            
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<u8>& data)
{
    for (auto it = data.begin(); it != data.end(); it++)
    {
        stream << *it;
    }
    
    return stream;
}

void Program::setTarget(TargetPtr target)
{
    m_target = target;
}

DataSection& Program::data(void)
{
    return m_data_section;
}

RopSection& Program::code(void)
{
    return m_rop_section;
}

TargetPtr Program::target(void)
{
    return m_target;
}

void Program::write(std::ostream& stream)
{
    RopFileHeader header;
    
    header.magic = ROPFILE_MAGIC;
    header.version = ROPFILE_VERSION(1, 0);
    header.reserved1 = 0;
    header.reserved2 = 0;
    
    // deal with data section
    header.dsize = data().size();
    header.daddr = data().base();
    
    // deal with rop section
    std::vector<u8> rop_binary = generate_rop_binary();
    header.csize = rop_binary.size();
    header.centry = m_target->getCallerGadget()->address();
    
    stream << header << data().data() << rop_binary;
}

std::vector<u8> Program::generate_rop_binary(void)
{ 
    RopFunctionPtrList functions = code().functions();
    
    // find the entry function
    auto it = std::find_if(functions.begin(), functions.end(), [=](RopFunctionPtr func)
    {
        return (func->name() == "entry");
    });
    
    // if no entry then big problem
    if (it == functions.end())
    {
        throw std::runtime_error("No entry function defined");
    }
    
    RopFunctionPtr func = *it;
    
    // we only support a single function currently
    return func->binary(m_target);
}
