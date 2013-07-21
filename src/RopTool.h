#ifndef _ROPTOOL_H_
#define _ROPTOOL_H_

// boost
#include <boost/program_options.hpp>

// std
#include <memory>

class RopTool
{
    public:
        int start(int args, char *argv[]);
        
    private:
        typedef std::shared_ptr<boost::program_options::options_description> cmd_options;
        
        // setup the command line options
        cmd_options get_options(void);
        
        // we have a variable map and options
        boost::program_options::variables_map m_vm;
        cmd_options m_options;
};

#endif // _ROPTOOL_H_
