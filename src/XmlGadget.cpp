// roptool
#include "XmlGadget.h"

// std
#include <cstdlib>
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

int XmlGadget::address(void)
{
	return m_address;
}

void XmlGadget::set_address(const std::string& address)
{
	// use strtoul for conversion.
	m_address = std::strtoul(address.c_str(), NULL, 0);
}

XmlGadget::XmlGadget(void)
{
	using namespace std::placeholders;
	
	// create visitor
	m_visitor.reset(new XmlActionVisitor());
	
	// add handlers
	m_visitor->addHandler("address", std::bind(&XmlGadget::set_address, this, _1), true);
}

XmlGadget::~XmlGadget(void)
{

}
