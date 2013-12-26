// roptool
#include "XmlTargetManifest.h"

// std
#include <iostream>
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
class XmlTargetManifest::XmlTargetManifestVisitor : public tinyxml2::XMLVisitor
{
	public:
		XmlTargetManifestVisitor(void) { }
		
		// get/set manifest
		void setManifest(XmlTargetManifest *manifest) { m_manifest = manifest; }
		XmlTargetManifest *manifest(void) { return m_manifest; }
		
		bool VisitEnter(const tinyxml2::XMLDocument& doc)
		{
			std::cout << "VISIT DOC\n";
			return true;
		}
		
		bool VisitExit(const tinyxml2::XMLDocument& doc)
		{
			std::cout << "EXIT DOC\n";
			return true;
		}
		
		bool VisitEnter(const tinyxml2::XMLElement &ele, const tinyxml2::XMLAttribute *attr)
		{
			std::cout << "enter element: " << ele.GetText() << " attr: " << attr->Name() << std::endl;
			return true;
		}
		
		bool VisitExit(const tinyxml2::XMLElement &ele)
		{
			std::cout << "exit element: " << ele.GetText() << std::endl;
			return true;
		}		

		bool Visit(const tinyxml2::XMLDeclaration& decl)
		{
			std::cout << "xml decl\n";
			return true;
		}
		
		bool Visit(const tinyxml2::XMLText& text)
		{
			std::cout << "xml text\n";
			return true;
		}
		
		bool Visit(const tinyxml2::XMLComment& comment)
		{
			std::cout << "xml comment\n";
			return true;
		}
		
		bool Visit(const tinyxml2::XMLUnknown& unknown)
		{
			std::cout << "xml unk\n";
			return true;
		}
		
	private:
		XmlTargetManifest *m_manifest;
};

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
	
	// use visitor through XML
	m_xmldoc->Accept(m_visitor.get());
	return true;
}

XmlTargetManifest::XmlTargetManifest(void)
{
	// create visitor
	m_visitor.reset(new XmlTargetManifestVisitor());
	
	// set the manifest
	m_visitor->setManifest(this);
}

XmlTargetManifest::~XmlTargetManifest(void)
{

}
