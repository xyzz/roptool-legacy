// roptool
#include "DataSection.h"

// std
#include <algorithm>

void DataSection::setBase(u64 base_address)
{
    // go through whole list
    std::for_each(m_update_list.begin(), m_update_list.end(), [=](DataRefPtr ref)
    {
        ref->set(base_address+(ref->value()-m_base_address));
        ref->onUpdate(ref);
    });
    
    m_base_address = base_address;
}

DataRefPtr DataSection::add(u64 constant)
{
    // add a constant
    // construct a new data reference
    DataRefPtr ref(new DataRef(m_ref_id_count++));
    
    // constant value, no need for update notify
    ref->set(constant);
    return ref;
}

DataRefPtr DataSection::add(const std::string& str)
{
    DataRefPtr ref(new DataRef(m_ref_id_count++));
    
    // set an update notification
    m_update_list.push_back(ref);
    
    // get current position
    u64 offset = m_data.size();
    
    // allocate memory
    std::vector<unsigned char> data_temp(str.size());
    std::transform(str.begin(), str.end(), data_temp.begin(), [=](char c)
    {
        return static_cast<unsigned char>(c);
    });
    
    m_data.insert(m_data.end(), data_temp.begin(), data_temp.end());
    m_data.push_back('\0');
    
    // set value
    ref->set(m_base_address + offset);
    return ref;
}

u64 DataSection::size(void)
{
    return m_data.size();
}


u64 DataSection::base(void)
{
    return m_base_address;
}

const std::vector<u8>& DataSection::data(void)
{
    return m_data;
}

DataSection::DataSection(void)
{
    m_ref_id_count = 1;
}
