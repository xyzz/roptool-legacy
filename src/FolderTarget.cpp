// roptool
#include "FolderTarget.h"
#include "XmlGadget.h"
#include "XmlGadgetMap.h"
#include "XmlTargetManifest.h"

namespace fs = boost::filesystem;

FolderTarget::DirectoryList FolderTarget::read_directory(const fs::path& dir)
{
    DirectoryList list;
    
    // get a list of the files
    std::copy(fs::directory_iterator(dir), fs::directory_iterator(), std::back_inserter(list));
    return list;
}

void FolderTarget::readManifest(void)
{
    // create new manifest based off XML
    m_manifest.reset(new XmlTargetManifest);

    // parse the manifest
    m_manifest->parse(m_path + "/manifest.xml");
}

void FolderTarget::readGadgets(void)
{
    // empty gadget list
    m_gadgets.clear();
    
    // read directory
    DirectoryList list = read_directory(m_path + "/gadgets");
    
    // loop through the directory list
    std::for_each(list.begin(), list.end(), [=](const fs::path& p)
    {
        XmlGadgetPtr gadget(new XmlGadget);
        
        if (fs::is_regular_file(p))
        {
            // parse gadget
            gadget->parse(p.string());
            std::cout << "gadget: " << gadget->address() << std::endl;
        }
        
        // add to list
        m_gadgets.push_back(gadget);
    });
}

void FolderTarget::readGadgetMaps(void)
{
    // empty gadget list
    m_gadgetmaps.clear();
    
    // read directory
    DirectoryList list = read_directory(m_path + "/gadgetmaps");
    
    // loop through the directory list
    std::for_each(list.begin(), list.end(), [=](const fs::path& p)
    {
        XmlGadgetMapPtr gadgetmap(new XmlGadgetMap);
        
        if (fs::is_regular_file(p))
        {
            // parse gadgetmap
            gadgetmap->addGadgets(m_gadgets);
            gadgetmap->parse(p.string());
            std::cout << "gadgetmap\n";
        }
        
        // add to list
        m_gadgetmaps.push_back(gadgetmap);
    });
    
    // sort by size
    std::sort(m_gadgetmaps.begin(), m_gadgetmaps.end(), [=](GadgetMapPtr i, GadgetMapPtr j) -> bool
    {
        return (i->size() < j->size());
    });
}

GadgetMapPtr FolderTarget::bestGadgetMap(const std::string& prototype)
{
    // create regular expression
    GadgetMapPtr map;
    
    // loop through gadgetmaps
    auto it = std::find_if(m_gadgetmaps.begin(), m_gadgetmaps.end(), [=](GadgetMapPtr map) -> bool
    {
        boost::cmatch match;
        
        // first element that is valid should be smallest since list is sorted
        return (boost::regex_match(prototype.c_str(), match, map->regex()));
    });
    
    // check if beyond range
    if (it == m_gadgetmaps.end())
    {
        // no gadget found!
        return map;
    }
    
    // copy the gadgetmap
    map = (*it)->clone();
    return map;
}

GadgetPtr FolderTarget::getCallerGadget(void)
{
    // loop through gadgets
    auto it = std::find_if(m_gadgets.begin(), m_gadgets.end(), [=](GadgetPtr gadget) -> bool
    {
        return (gadget->name() == m_manifest->caller_gadget());
    });
    
    // check if we never got one
    if (it == m_gadgets.end())
    {
        std::string err;
        err = err + "Caller gadget \"" + m_manifest->caller_gadget() + "\" not found!";
        
        // caller gadget is not defined
        throw std::runtime_error(err.c_str()); 
    }
    
    // return the gadget
    return *it;
}

void FolderTarget::setName(const std::string& name)
{
    // set name
    m_name = name;
    m_path = target_folder() + "/" + name;
    
    // read manifest
    readManifest();
    readGadgets();
    readGadgetMaps();
}
