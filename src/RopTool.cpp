
// roptool
#include "RopTool.h"

// std
#include <iostream>

// namespace for ease of reading (and writing)
namespace po = boost::program_options;

RopTool::cmd_options RopTool::get_options(void)
{
    // check if there are any options already created by this function
    if (m_options)
    {
        // options already set, just return
        return m_options;
    }
    
    // create new options
    m_options.reset(new po::options_description());
    
    m_options->add_options()
        ("help,h", "Show this help dialog.");
        
    return m_options;
}
        
int RopTool::start(int argc, char *argv[])
{
    // get commandline options
    cmd_options options = get_options();
    
    try
    {
        // parse the command line
        po::store(po::parse_command_line(argc, argv, *options.get()), m_vm);
        po::notify(m_vm);
        
        // check for no args or the help
		if (m_vm.size() == 0 || m_vm.count("help"))
		{
			// show help
			std::cout << *options.get() << std::endl;
			return 1;
		}
    }
 
    // catch any exceptions
    catch(std::exception& e) 
    {
        // display the exception
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Use --target [target] --help for usage information for a target" << std::endl;
        std::cerr << "Else, pass no args for generic usage" << std::endl;
        return 1;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    RopTool roptool;
    return roptool.start(argc, argv);
}
