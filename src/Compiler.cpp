#include "Compiler.h"

void Compiler::visit(CallParameter *param)
{
	std::cout << "param:" << param->parameter() << std::endl;
}

void Compiler::visit(CallDecl *param)
{
	std::cout << "call:" << param->name() << std::endl;
}

void Compiler::visit(CodeDecl *param)
{
	std::cout << "code:" << param->name() << std::endl;
}

void Compiler::visit(FunctionDataDecl *param)
{
	std::cout << "Function:" << param->name() << " - " << param->getData() << std::endl;
}

void Compiler::visit(SymbolDataDecl *param)
{
	std::cout << "Symbol:" << param->name() << " - " << param->getData() << std::endl;
}

void Compiler::visit(DataDecl *param)
{
	std::cout << "DataDecl start" << std::endl;
}

void Compiler::visit(RopScript *param)
{
	std::cout << "RopScript start" << std::endl;
}
