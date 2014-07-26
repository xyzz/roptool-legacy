#ifndef _DATA_SECTION_H_
#define _DATA_SECTION_H_

// roptool
#include "Types.h"
#include "DataRef.h"

// std
#include <memory>
#include <string>
#include <vector>

class DataSection
{
    public:
        DataSection(void);
        
        void setBase(u64 base_address);
        
        // add constant
        DataRefPtr add(u64 constant);
        
        // add string
        DataRefPtr add(const std::string& str);
        
        // getters
        u64 size(void);
        u64 base(void);
        const std::vector<u8>& data(void);
        
    private:
        u64 m_ref_id_count, m_base_address;
        std::vector<unsigned char> m_data;
        DataRefPtrList m_update_list;
};

typedef std::shared_ptr<DataSection> DataSectionPtr;

#endif // _DATA_SECTION_H_
