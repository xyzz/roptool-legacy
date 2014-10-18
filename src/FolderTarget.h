#ifndef _FOLDER_TARGET_H_
#define _FOLDER_TARGET_H_

// roptool
#include "Target.h"
#include "Gadget.h"
#include "GadgetMap.h"

// std
#include <string>
#include <vector>

// boost
#include <boost/filesystem.hpp>

class FolderTarget : public Target
{
    public:
        FolderTarget(void) { }
        FolderTarget(const std::string& name) { setName(name); }
        
        // accessor/mutator
        const std::string& name(void) { return m_name; }
        void setName(const std::string& name);
        
        // 
        TargetManifestPtr manifest(void) { return m_manifest; }
        
        // get the best  gadget map
        GadgetMapPtr bestGadgetMap(const std::string& regex);
        
        // get the caller gadget
        GadgetPtr getCallerGadget(void);
        
        // check if target defines function
        bool isFunction(const std::string& function) const;
        
    private:
        std::string m_name;
        std::string m_path;
        GadgetPtrList m_gadgets;
        GadgetMapPtrList m_gadgetmaps;
        TargetManifestPtr m_manifest;
        
        void readManifest(void);
        void readGadgets(void);
        void readGadgetMaps(void);
        
        typedef std::vector<boost::filesystem::path> DirectoryList;
        DirectoryList read_directory(const boost::filesystem::path& dir);
};

#endif // _FOLDER_TARGET_H_