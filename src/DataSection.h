#ifndef _DATA_SECTION_H_
#define _DATA_SECTION_H_

#include "Types.h"

#include <functional>
#include <string>
#include <list>
#include <vector>
#include <memory>


class DataSection;
class DataRef;

typedef std::shared_ptr<DataRef> DataRefPtr;
typedef std::list<DataRefPtr> DataRefPtrList;

typedef std::function<void(DataRefPtr)> DataRefHandler;

class DataRef
{
    friend class DataSection;
    
    public:
        u64 refId(void) { return m_ref_id; }
        u64 value(void) { return m_val; }
        void addValueChangeHandler(DataRefHandler func) { m_handler.push_back(func); }
    
    protected:
        void set(u64 val) { m_val = val; }
        
    private:
        // only friends can construct these
        DataRef(u64 ref_id) { m_ref_id = ref_id; }
        
        void onUpdate(DataRefPtr ref)
        {
            std::for_each(m_handler.begin(), m_handler.end(), [=](DataRefHandler handler)
            {
                handler(ref);
            });
        }
        
        std::list<DataRefHandler> m_handler;
        u64 m_ref_id, m_val;
};

class DataSection
{
    public:
        DataSection(void)
        {
            m_ref_id_count = 1;
        }
        
        void setBase(u64 base_address)
        {
            // go through whole list
            std::for_each(m_update_list.begin(), m_update_list.end(), [=](DataRefPtr ref)
            {
                ref->set(base_address+(ref->value()-m_base_address));
                ref->onUpdate(ref);
            });
            
            m_base_address = base_address;
        }
        
        DataRefPtr add(u64 constant)
        {
            // add a constant
            // construct a new data reference
            DataRefPtr ref(new DataRef(m_ref_id_count++));
            
            // constant value, no need for update notify
            ref->set(constant);
            return ref;
        }
        
        DataRefPtr add(const std::string& str)
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
        
    private:
        u64 m_ref_id_count, m_base_address;
        std::vector<unsigned char> m_data;
        std::list<DataRefPtr> m_update_list;
};

#endif // _DATA_SECTION_H_
