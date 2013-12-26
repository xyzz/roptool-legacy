// roptool
#include "XmlTargetManifest.h"

// tinyxml2
#include <tinyxml2.h>

bool XmlTargetManifest::setFile(const std::string& file)
{
	m_xmldoc.reset(new tinyxml2::XMLDocument);
	return true;
}

XmlTargetManifest::XmlTargetManifest()
{

}

XmlTargetManifest::~XmlTargetManifest()
{

}
