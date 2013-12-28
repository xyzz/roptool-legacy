#ifndef _XML_ACTION_VISITOR_H_
#define _XML_ACTION_VISITOR_H_

// std
#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <vector>

// tinyxml2
#include <tinyxml2.h>

typedef std::function<void(const std::string&)> VisitorAction;

class XmlActionVisitor : public tinyxml2::XMLVisitor
{
	public:
		XmlActionVisitor(void) { }

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
		std::vector<std::string> m_stack;
		std::map<std::string, VisitorAction> m_actions;
};

#endif // _XML_ACTION_VISITOR_H_
