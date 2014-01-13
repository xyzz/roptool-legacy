#ifndef _XML_GADGET_MAP_H_
#define _XML_GADGET_MAP_H_

// roptool
#include "GadgetMap.h"
#include "XmlActionVisitor.h"

// std
#include <memory>
#include <string>
#include <vector>

// forward declare class
namespace tinyxml2
{
	class XMLDocument;
}

class XmlGadgetMap;
typedef std::shared_ptr<XmlGadgetMap> XmlGadgetMapPtr;

class XmlGadgetMap : public GadgetMap
{
	public:
		XmlGadgetMap(void);
		~XmlGadgetMap(void);
		
		bool parse(const std::string& file);
		
		boost::regex regex(void);
		void addGadgets(GadgetPtrList gadgets);
		int size(void) { return m_stack.size(); }
		
		// setters
		void setFunction(Function address);
		void setParameters(DataRefPtrList refs);
		
		std::vector<int> stack(void);
		
		GadgetMapPtr clone(void) const { return XmlGadgetMapPtr(new XmlGadgetMap(*this)); }
		
	private:
		std::unique_ptr<XmlActionVisitor> m_visitor;
		std::unique_ptr<tinyxml2::XMLDocument> m_xmldoc;
		
		GadgetPtrList m_gadgets;
		boost::regex m_regex;
		std::list<std::string> m_stack;
		std::map<std::string, int> m_definitions;
		
		
		XmlGadgetMap(const XmlGadgetMap& rhs);
		
		void set_regex(const std::string& regex_str);
		void add_stack_data(const std::string& stack_data);
};

#endif // _XML_GADGET_MAP_H_
