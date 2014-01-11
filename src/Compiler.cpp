#include "Compiler.h"

void Compiler::store_param(u64 value, int val_bits, int arch_bits)
{
	// check parameter size
	if (val_bits > arch_bits)
	{
		// this parameter is larger than the arch!
		// we need to store it in adjacent parameters
		int param_n = val_bits / arch_bits;
		
		// align to parameter of 2
		if (m_param_type.size() % 2)
		{
			m_param_type.push_back('v');
			m_param.push_back(0);
		}
		
		for (int i = 0; i < param_n; i++)
		{
			m_param_type.push_back('v');
			m_param.push_back((value >> ((param_n-i-1) * arch_bits)) & (arch_bits-1));
		}
	}
	else
	{	
		// just push onto list
		m_param_type.push_back('v');
		m_param.push_back(value & (arch_bits-1));
	}	
}

void Compiler::visit(StringParameter *param)
{
	// add string to data section
	m_param_type.push_back('v');
	m_param.push_back(0);
}

void Compiler::visit(ConstantParameter *param)
{
	int param_bits = param->bitlen();
	int arch_bits = m_target->manifest()->arch_bitlen();
	
	// store param
	store_param(param->value(), param_bits, arch_bits);
}

void Compiler::visit(ReturnParameter *param)
{
	int param_bits = param->bitlen();
	int arch_bits = m_target->manifest()->arch_bitlen();
	
	// store param
	store_param(0, param_bits, arch_bits);
	
	// modify last type to type 'r'
	m_param_type.pop_back();
	m_param_type.push_back('r');
}

void Compiler::visit(InlineLoadParameter *param)
{
	int param_bits = param->bitlen();
	int arch_bits = m_target->manifest()->arch_bitlen();
	
	// store param
	store_param(param->value(), param_bits, arch_bits);
	
	// modify last type to type 'l'
	m_param_type.pop_back();
	m_param_type.push_back('l');
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
	
	std::cout << "call prototype: " << call_prototype << "\n";
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
	m_target = target;
	ast->traverse(this);
}
