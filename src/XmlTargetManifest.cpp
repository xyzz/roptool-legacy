// roptool
#include "XmlTargetManifest.h"

// std
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <map>
#include <stdexcept>

// tinyxml2
#include <tinyxml2.h>


typedef std::function<void(const std::string&)> VisitorAction;

class XmlTargetManifest::XmlTargetManifestVisitor : public tinyxml2::XMLVisitor
{
	public:
		XmlTargetManifestVisitor(void) { }
		
		// get/set manifest
		void setManifest(XmlTargetManifest *manifest) { m_manifest = manifest; }
		XmlTargetManifest *manifest(void) { return m_manifest; }

		// add handler
		void addHandler(const std::string& event, VisitorAction f)
		{
			m_actions[event] = f;
		}
		
		bool VisitEnter(const tinyxml2::XMLDocument& doc)
		{
			//std::cout << "VISIT DOC\n";
			return true;
		}
		
		bool VisitExit(const tinyxml2::XMLDocument& doc)
		{
			//std::cout << "EXIT DOC\n";
			return true;
		}
		
		bool VisitEnter(const tinyxml2::XMLElement &ele, const tinyxml2::XMLAttribute *attr)
		{
			//std::cout << "enter element: \"" << ele.Name() << "\"\n";// << attr->Name() << std::endl;
			//manifest->set
			m_stack.push_back(ele.Name());
			return true;
		}
		
		bool VisitExit(const tinyxml2::XMLElement &ele)
		{
			//std::cout << "exit element: " << ele.Name() << std::endl;
			m_stack.pop_back();
			return true;
		}		

		bool Visit(const tinyxml2::XMLDeclaration& decl)
		{
			//std::cout << "xml decl\n";
			return true;
		}
		
		bool Visit(const tinyxml2::XMLText& text)
		{
			std::string action;
			std::for_each(m_stack.begin(), m_stack.end(), [=, &action](const std::string& str)
			{
				action.append(str);
				action.append("-");
			});
			
			action.pop_back();
			
			try
			{
				m_actions.at(action)(text.Value());
			}
			catch (const std::out_of_range& e)
			{
				// ignore
			}
			
			return true;
		}
		
		bool Visit(const tinyxml2::XMLComment& comment)
		{
			//std::cout << "xml comment\n";
			return true;
		}
		
		bool Visit(const tinyxml2::XMLUnknown& unknown)
		{
			//std::cout << "xml unk\n";
			return true;
		}
		
	private:
		XmlTargetManifest *m_manifest;
		std::vector<std::string> m_stack;
		std::map<std::string, VisitorAction> m_actions;
};

bool XmlTargetManifest::parseFile(const std::string& target)
{
	// read file
	//std::string xml = read_file(file);
	std::string file = target + "/manifest.xml";
	
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

XmlTargetManifest::XmlTargetManifest(void)
{
	using namespace std::placeholders;
	
	// create visitor
	m_visitor.reset(new XmlTargetManifestVisitor());
	
	// set the manifest
	m_visitor->setManifest(this);
	
	// add handlers
	VisitorAction set_version = std::bind(&XmlTargetManifest::set_version, this, _1);
	m_visitor->addHandler("version", set_version);
	
}

XmlTargetManifest::~XmlTargetManifest(void)
{

}
