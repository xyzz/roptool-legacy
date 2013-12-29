#include "Compiler.h"

class SymbolVisitor : public boost::static_visitor<unsigned int>
{
	public:	
		SymbolVisitor(std::map<std::string, unsigned int> *map)
		{
			m_map = map;
		}
		
		unsigned int operator()(unsigned int i) const
		{
			return i;
		}
		
		unsigned int operator()(const std::string& str) const
		{
			// look up symbol
			auto it = m_map->find(str);
			
			if (it == m_map->end())
			{
				throw std::runtime_error("Symbol resolves to non-existant definition.\n");
			}
			
			return it->second;
		}
	
	private:
		std::map<std::string, unsigned int> *m_map;
};

void Compiler::visit(CallParameter *param)
{
	//std::cout << "param:" << param->parameter() << std::endl;
}

void Compiler::visit_enter(CallDecl *param)
{
	//std::cout << "call:" << param->name() << std::endl;
}

void Compiler::visit_exit(CallDecl *param)
{
	//std::cout << "call:" << param->name() << std::endl;
}

void Compiler::visit_enter(CodeDecl *param)
{
	//std::cout << "code:" << param->name() << std::endl;
}

void Compiler::visit_exit(CodeDecl *param)
{
	//std::cout << "code:" << param->name() << std::endl;
}

void Compiler::visit(FunctionDataDecl *param)
{
	bool redefined = !m_functions.insert(std::pair<std::string, unsigned int>(param->name(), param->getData())).second;
	
	if (redefined)
	{
		std::cout << "function '" << param->name() << "' redefined!" << std::endl;
	}
}

void Compiler::visit(SymbolDataDecl *param)
{
	// add symbol to map
	auto val = boost::apply_visitor(*m_symbol_visitor, param->getData());
	bool redefined = !m_symbols.insert(std::pair<std::string, unsigned int>(param->name(), val)).second;
	
	if (redefined)
	{
		std::cout << "symbol '" << param->name() << "' redefined!" << std::endl;
	}
	
	//std::cout << "Symbol:" << param->name() << " - " << param->getData() << std::endl;
}

void Compiler::visit(DataDecl *param)
{
	// 
	m_symbols.clear();
	m_functions.clear();
	
	m_symbol_visitor.reset(new SymbolVisitor(&m_symbols));
	
	std::cout << "DataDecl start" << std::endl;
}

void Compiler::visit_enter(RopScript *param)
{
	// create new data section and code section
	//m_data.reset(new DataSection());
	//m_code.reset(new CodeSection());
	std::cout << "RopScript start" << std::endl;
}

void Compiler::visit_exit(RopScript *param)
{
	// create new data section and code section
	//m_data.reset(new DataSection());
	//m_code.reset(new CodeSection());
	std::cout << "RopScript exit" << std::endl;
}

void Compiler::compile(VisitablePtr ast, TargetPtr target)
{
	ast->traverse(this);
}
