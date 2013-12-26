// roptool
#include "XmlTargetManifest.h"

// std
#include <stdexcept>

// tinyxml2
#include <tinyxml2.h>

/*
std::string read_file(const std::string& file)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	
	if (!in)
	{
		// \TODO create error message
		// could not open file
		throw std::runtime_error("Could not open manifest file.");
	}
	
	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	
	return contents;
} */

bool XmlTargetManifest::parseFile(const std::string& file)
{
	// read file
	//std::string xml = read_file(file);
	
	// create new XML document
	m_xmldoc.reset(new tinyxml2::XMLDocument);
	
	// load XML file
	if (m_xmldoc->LoadFile(file.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		// \TODO: better error message
		// error reading file
		throw std::runtime_error("Could not open manifest file.");
	}
	
	return true;
}

XmlTargetManifest::XmlTargetManifest(void)
{

}

XmlTargetManifest::~XmlTargetManifest(void)
{

}
