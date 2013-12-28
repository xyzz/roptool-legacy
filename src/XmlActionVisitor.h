#ifndef _XML_ACTION_VISITOR_H_
#define _XML_ACTION_VISITOR_H_

// std
#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>

// tinyxml2
#include <tinyxml2.h>

typedef std::function<void(const std::string&)> VisitorAction;

class uncalled_action_error : public std::runtime_error
{
	public:
		uncalled_action_error(const std::string& action) : std::runtime_error("Could not find element for action:" + action)
		{
			m_action = action;
		}
		
		const std::string& action(void)
		{
			return m_action;
		}
		
	private:
		std::string m_action;
};

class XmlActionVisitor : public tinyxml2::XMLVisitor
{
	public:
		XmlActionVisitor(void) { }

		// add handler
		void addHandler(const std::string& event, VisitorAction f, bool is_req = false)
		{
			m_actions[event] = f;
			
			if (is_req)
			{
				m_req_action.insert(event);
			}
		}
		
		bool VisitEnter(const tinyxml2::XMLDocument& doc)
		{
			//std::cout << "VISIT DOC\n";
			m_used_actions.clear();
			return true;
		}
		
		bool VisitExit(const tinyxml2::XMLDocument& doc)
		{
			//std::cout << "EXIT DOC\n";
			
			std::for_each(m_req_action.begin(), m_req_action.end(), [=](const std::string& action)
			{
				// check if a required action has NOT been used
				if (m_used_actions.count(action) == 0)
				{
					// throw exception
					throw uncalled_action_error(action);
				}
			});
			
			m_used_actions.clear();
			return true;
		}
		
		bool VisitEnter(const tinyxml2::XMLElement &ele, const tinyxml2::XMLAttribute *attr)
		{
			//std::cout << "enter element: \"" << ele.Name() << "\"\n";// << attr->Name() << std::endl;
			//manifest->se
			
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
				m_used_actions.insert(action);
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
		std::unordered_set<std::string> m_used_actions;
		std::unordered_set<std::string> m_req_action;
		std::map<std::string, VisitorAction> m_actions;
};

#endif // _XML_ACTION_VISITOR_H_
