// roptool
#include "XmlGadgetMap.h"

// std
#include <cstdlib>
#include <stdexcept>

// tinyxml2
#include <tinyxml2.h>

bool XmlGadgetMap::parse(const std::string& file)
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

void XmlGadgetMap::addGadgets(GadgetPtrList gadgets)
{
	m_gadgets = gadgets;
}

boost::regex XmlGadgetMap::regex(void)
{
	return m_regex;
}

void XmlGadgetMap::set_regex(const std::string& regex_str)
{
	m_regex = regex_str;
}

void XmlGadgetMap::stack(void)
{
	// create regex expr
	//boost::regex arg = 
	boost::regex gadget_ref("GADGET_ADDRESS\\(([A-Z0-9a-z._%+-]+)\\)");
	boost::cmatch match;
	
	// loop through the stack
	for (auto it = m_stack.begin(); it != m_stack.end(); it++)
	{
		int value = 0;
		
		// lets see if value has been defined
		if (m_definitions.find(*it) != m_definitions.end())
		{
			value = m_definitions[*it];
		}
		
		// check if its a gadget reference
		else if (boost::regex_match((*it).c_str(), match, gadget_ref))
		{
			auto gadget_it = std::find_if(m_gadgets.begin(), m_gadgets.end(), [=](const GadgetPtr& gadget)
			{
				if (gadget->name() == match[1].str())
				{
					// found gadget
					return true;
				}
				
				return false;
			});
			
			// check if no gadget was found
			if (gadget_it == m_gadgets.end())
			{
				// \TODO: we need to do better responses
				throw std::runtime_error("Error, no reference to gadget: " + match[1].str());
			}
		}
		
		// check if its a numeric value
		//else if ()
		//{
		
		//}
		
		// default
		else
		{
		
		}
	}
	
}

void XmlGadgetMap::add_stack_data(const std::string& stack_str)
{
	// stack_str:
	//	either a VALUE
	//	keyword (FUNC_ADDR, ARG*)
	//	gadget reference GADGET( GADGNET NAME )
	
	// eventually all will be integer value
	m_stack.push_back(stack_str);
}

XmlGadgetMap::XmlGadgetMap(void)
{
	using namespace std::placeholders;
	
	// create visitor
	m_visitor.reset(new XmlActionVisitor());
	
	// add handlers
	m_visitor->addHandler("gadgetmap-regex", std::bind(&XmlGadgetMap::set_regex, this, _1), true);
	m_visitor->addHandler("gadgetmap-stack-data", std::bind(&XmlGadgetMap::add_stack_data, this, _1), true);
}

XmlGadgetMap::~XmlGadgetMap(void)
{

}
