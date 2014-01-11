// roptool
#include "XmlTargetManifest.h"
#include "XmlActionVisitor.h"

// std
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <map>
#include <stdexcept>

// boost
#include <boost/lexical_cast.hpp>

// tinyxml2
#include <tinyxml2.h>

bool XmlTargetManifest::parse(const std::string& file)
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

void XmlTargetManifest::set_version(const std::string& version)
{
	std::cout << "setting version: \"" << version << "\"\n";
}

void XmlTargetManifest::set_arch_bitlen(const std::string& bitlen)
{
	std::cout << "setting architecture bitlen: \"" << bitlen << "\"\n";
	
	// get integer value
	try
	{
		m_bitlen = boost::lexical_cast<int>(bitlen);
	}
	
	catch (const boost::bad_lexical_cast&)
	{
		// this is a big error, throw exception
		// \TODO: error handling
		throw std::runtime_error("Error, Architecture bitlen in target is invalid integer.");
	}
}

XmlTargetManifest::XmlTargetManifest(void)
{
	using namespace std::placeholders;
	
	// create visitor
	m_visitor.reset(new XmlActionVisitor());
	
	// add handlers
	VisitorAction set_version = std::bind(&XmlTargetManifest::set_version, this, _1);
	VisitorAction set_arch_bitlen = std::bind(&XmlTargetManifest::set_arch_bitlen, this, _1);
	m_visitor->addHandler("version", set_version, true);
	m_visitor->addHandler("arch_bitlen", set_arch_bitlen, true);
}

XmlTargetManifest::~XmlTargetManifest(void)
{

}
