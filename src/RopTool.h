#ifndef _ROPTOOL_H_
#define _ROPTOOL_H_

// roptool
#include "Target.h"

// boost
#include <boost/program_options.hpp>

// std
#include <memory>
#include <string>

class RopTool
{
    public:
        int start(int args, char *argv[]);
        
    private:
        typedef std::shared_ptr<boost::program_options::options_description> cmd_options;
        
        // setup the command line options
        cmd_options get_options(void);
        void set_target(const std::string& target);
        void set_source(const std::string& source);
        void set_output(const std::string& output);
        void set_base(const std::string& address);
        void set_sled_length(const std::string& sled_length);
        
        // variables
        TargetPtr target;
        std::string source, output;
        u64 base_address;
        u32 m_sled_length;
        
        // we have a variable map and options
        boost::program_options::variables_map m_vm;
        cmd_options m_options;
};

#endif // _ROPTOOL_H_
