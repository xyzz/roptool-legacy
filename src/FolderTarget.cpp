// roptool
#include "FolderTarget.h"
#include "XmlTargetManifest.h"

void FolderTarget::setName(const std::string& name)
{
	// set name
	m_name = name;
	
	// create new manifest based off XML
	m_manifest.reset(new XmlTargetManifest);

	// parse the manifest
	m_manifest->parseFile(target_folder()+"/"+name);
}
