#ifndef _XML_GADGET_H_
#define _XML_GADGET_H_

// roptool
#include "Gadget.h"
#include "XmlActionVisitor.h"

// std
#include <memory>
#include <string>

// forward declare class
namespace tinyxml2
{
	class XMLDocument;
}

class XmlGadget : public Gadget
{
	public:
		XmlGadget(void);
		~XmlGadget(void);
		
		bool parse(const std::string& file);
		
		int address(void);
		void set_address(const std::string& str_address);
		
	private:
		std::unique_ptr<XmlActionVisitor> m_visitor;
		std::unique_ptr<tinyxml2::XMLDocument> m_xmldoc;
		
		int m_address;
};

typedef std::shared_ptr<XmlGadget> XmlGadgetPtr;

#endif // _XML_GADGET_H_
