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
