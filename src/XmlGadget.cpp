// roptool
#include "XmlGadget.h"

// std
#include <stdexcept>

// tinyxml2
#include <tinyxml2.h>

bool XmlGadget::parse(const std::string& file)
{
	// create new XML document
	m_xmldoc.reset(new tinyxml2::XMLDocument);
	
	// load XML file
	if (m_xmldoc->LoadFile(file.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		// \TODO: better error message
		// error reading file
		throw std::runtime_error(std::string("Could not open manifest file: ") + file);
	}
	
	// use visitor through XML
	m_xmldoc->Accept(m_visitor.get());
	return true;
}

XmlGadget::XmlGadget(void)
{
	using namespace std::placeholders;
	
	// create visitor
	m_visitor.reset(new XmlActionVisitor());
	
	// add handlers
}

XmlGadget::~XmlGadget(void)
{

}
