#ifndef _PROGRAM_H_
#define _PROGRAM_H_

// roptool
#include "Types.h"
#include "DataSection.h"
#include "RopSection.h"

// std
#include <ostream>
#include <memory>

#define ROPFILE_MAGIC           (0x7E504F52)
#define ROPFILE_VERSION(x,y)    ((x << 0x10) | (y & 0xFFFF))

typedef struct
{
    u32 magic;              // 0x00
    u32 version;            // 0x04
    u32 reserved1;          // 0x08
    u32 reserved2;          // 0x0C
    u64 dsize;              // 0x10
    u64 daddr;              // 0x18
    u64 csize;              // 0x20
    u64 centry;             // 0x28
} RopFileHeader;

class Program
{
    public:
        // write only
        void write(std::ostream& stream);
        
        void addSled(u32 length);
        
        // mutators
        void setTarget(TargetPtr target);
        
        // accessors
        DataSection& data(void);
        RopSection& code(void);
        TargetPtr target(void);
        
    private:
        DataSection m_data_section;
        RopSection m_rop_section;
        TargetPtr m_target;
        std::vector<u8> m_sled;
        
        std::vector<u8> generate_rop_binary(void);
};

typedef std::shared_ptr<Program> ProgramPtr;

#endif // _PROGRAM_H_
