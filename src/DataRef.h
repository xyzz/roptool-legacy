#ifndef _DATA_REF_H_
#define _DATA_REF_H_

// roptool
#include "Types.h"

// std
#include <algorithm>
#include <functional>
#include <list>
#include <memory>

// forward declare classes
class DataSection;
class DataRef;

// define smart pointer types
typedef std::shared_ptr<DataRef> DataRefPtr;
typedef std::list<DataRefPtr> DataRefPtrList;

// handler
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

#endif // _DATA_REF_H_
