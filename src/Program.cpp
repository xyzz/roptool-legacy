#include "Program.h"

std::ostream& operator<<(std::ostream& stream, const RopFileHeader& header)
{
    stream.write(reinterpret_cast<const char *>(&header), sizeof(RopFileHeader));
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<u8>& data)
{
    std::ostream_iterator<u8> output_iterator(stream);
    std::copy(data.begin(), data.end(), output_iterator);
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

void Program::addSled(u32 length)
{
    m_sled.clear();
    
    // get the caller gadget
    u64 addr = m_target->getCallerGadget()->address();
    const u8 *p = reinterpret_cast<const u8 *>(&addr);
    
    // calculate the number of writes
    int writes = length;
    int arch_bytes = m_target->manifest()->arch_bitlen()/8;
    int align = m_target->manifest()->stack_alignment();
    while ((writes % arch_bytes) || (writes % align)) writes++;
    
    // add a sled of the specified length
    for (int i = 0; i < writes; i++)
    {
        m_sled.push_back(p[i % arch_bytes]);
    }
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
    header.csize = m_sled.size() + rop_binary.size();
    header.centry = m_target->getCallerGadget()->address();
    
    stream << header << data().data() << m_sled << rop_binary;
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
