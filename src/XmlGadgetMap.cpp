// roptool
#include "XmlGadgetMap.h"

// std
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

// boost
#include <boost/lexical_cast.hpp>

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

std::vector<u64> XmlGadgetMap::stack(void)
{
	boost::regex gadget_ref("GADGET_ADDRESS\\(([A-Z0-9a-z._%+-]+)\\)");
	boost::regex number("\\s*[+-]?([1-9][0-9]*|0[0-7]*|0[xX][0-9a-fA-F]+)");
	boost::cmatch match;
	std::vector<u64> data;
	
	// loop through the stack
	for (auto it = m_stack.begin(); it != m_stack.end(); it++)
	{
		u64 value = 0;
		
		// lets see if value has been defined
		if (m_definitions.find(*it) != m_definitions.end())
		{
			value = m_definitions[*it];
			std::cout << "value: " << value << "\n";
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
			
			// get the address
			value = (*gadget_it)->address();
			std::cout << "gadget: " << value << "\n";
		}
		
		// check if its a numeric value
		else if (boost::regex_match((*it).c_str(), match, number))
		{
			// is number
			std::cout << "is number - " << (*it) << "\n";
			value = strtoull((*it).c_str(), NULL, 0);
		}
		
		// default
		else
		{
			std::cout << "unknown - " << (*it) << "\n";
			throw std::runtime_error("Error, gadget map has undefined name: " + (*it));
		}
		
		// add to data structure
		data.push_back(value);
	}
	
	// return stack
	return data;
}


void XmlGadgetMap::setFunction(Function address)
{
	// add definition
	m_definitions["FUNC_ADDRESS"] = address;
}

void XmlGadgetMap::setParameters(DataRefPtrList refs)
{
	int i = 0;
	
	std::for_each(refs.begin(), refs.end(), [&](const DataRefPtr ref)
	{
		std::string arg_num = "ARG" + boost::lexical_cast<std::string>(i++);
		m_definitions[arg_num] = ref->value();
		
		ref->addValueChangeHandler([=](DataRefPtr p)
		{
			m_definitions[arg_num] = p->value();
		});
	});
}

void XmlGadgetMap::add_stack_data(const std::string& stack_str)
{
	// stack_str:
	//	either a VALUE
	//	keyword (FUNC_ADDR, ARG*)
	//	gadget reference GADGET( GADGNET NAME )
	boost::regex arg_regex("ARG[0-9]+");
	boost::cmatch match;
	
	// check if its an argument
	if (boost::regex_match(stack_str.c_str(), match, arg_regex))
	{
		// add to our definitions map with default of zero
		m_definitions.insert(std::pair<std::string, int>(stack_str, 0));
	}
	
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

XmlGadgetMap::XmlGadgetMap(const XmlGadgetMap& rhs)
{
	using namespace std::placeholders;
	
	// create visitor
	m_visitor.reset(new XmlActionVisitor());
	
	// add handlers
	m_visitor->addHandler("gadgetmap-regex", std::bind(&XmlGadgetMap::set_regex, this, _1), true);
	m_visitor->addHandler("gadgetmap-stack-data", std::bind(&XmlGadgetMap::add_stack_data, this, _1), true);

	// copy data
	m_stack = rhs.m_stack;
	m_gadgets = rhs.m_gadgets;
	m_regex = rhs.m_regex;
	m_definitions = rhs.m_definitions;
}

XmlGadgetMap::~XmlGadgetMap(void)
{

}
