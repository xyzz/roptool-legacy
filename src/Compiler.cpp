#include "Compiler.h"

void Compiler::visit(StringParameter *param)
{
	// add string to data section
	m_param_type.push_back('v');
	m_param.push_back(0);
}

void Compiler::visit(ConstantParameter *param)
{
	// just push onto list
	m_param_type.push_back('v');
	m_param.push_back(param->value());
	std::cout << "got constant value: " << param->value() << "\n";
}

void Compiler::visit(ReturnParameter *param)
{
	m_param_type.push_back('r');
	m_param.push_back(0);
}

void Compiler::visit(InlineLoadParameter *param)
{
	// special type of 'l'
	m_param_type.push_back('l');
	m_param.push_back(param->value());
}

void Compiler::visit_enter(CallDecl *param)
{
	m_param_type.clear();
	m_param.clear();
}

void Compiler::visit_exit(CallDecl *param)
{
	// add a null terminator and construct string
	m_param_type.push_back('\0');
	std::string param_type_list(m_param_type.data());
	std::string call_prototype = std::string("(") + param_type_list + ")";
	
	
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
	bool redefined = !m_functions.insert(std::pair<std::string, Function>(param->name(), param->getData())).second;
	
	if (redefined)
	{
		std::cout << "function '" << param->name() << "' redefined!" << std::endl;
	}
}

void Compiler::visit(DataDecl *param)
{
	// 
	m_functions.clear();
	
	
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
