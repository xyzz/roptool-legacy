// roptool
#include "FolderTarget.h"
#include "XmlTargetManifest.h"

TargetManifestPtr FolderTarget::manifest(void)
{
	// create new manifest based off XML
	TargetManifestPtr manifest(new XmlTargetManifest);

	// \TODO: do things
	return manifest;
}
