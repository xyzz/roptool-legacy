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

void Compiler::visit(SymbolParameter *param)
{
	std::cout << "param: got symbol - " << param->value() << "\n";
}

void Compiler::visit(StringParameter *param)
{
	std::cout << "param: got string - " << param->value() << "\n";
}

void Compiler::visit(ConstantParameter *param)
{
	std::cout << "param: got constant - " << param->value() << "\n";
}

void Compiler::visit(ReturnParameter *param)
{		
	std::cout << "param: RETURN ADDRESS\n";
}
void Compiler::visit(InlineLoadParameter *param)
{
	std::cout << "param: got inline load at - " << param->value() << "\n";
	
	// check if parameter needs storage
	// these values are typical of strings
/*	unsigned int value = 0;
	char code = 'v';
	
	// check the parameter type
	switch (param->type())
	{
		case CONSTANT:
			code = 'v';
			value = 0;
			break;
			
		case STRING:
			code = 'v';
			// allocate memory for string
			value = 1;
			break;
		
		case SYMBOL:
			code = 'v';
			// lookup symbol
			value = 2;
			break;
			
		case RETURN:
			code = 'r';
			value = 3;
			break;
			
		case LOAD:
			code = 'l';
			value = 4;
			break;
	}
	*/
	//m_params.push_back(std::vector<char, unsigned int>(code, value));
	//std::cout << "param:" << param->parameter() << std::endl;
}

void Compiler::visit_enter(CallDecl *param)
{
	//m_params.clear();
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
