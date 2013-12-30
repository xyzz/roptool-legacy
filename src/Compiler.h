#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "AST.h"
#include "Target.h"

// std
#include <memory>
#include <string>

// forward declare class
class SymbolVisitor;

class Compiler : public ASTVisitor
{
	public:
		// CTOR
		Compiler(void) { }
		
		// compile
		void compile(VisitablePtr m_ast, TargetPtr m_target);
		
		// visitor methods
        void visit(SymbolParameter *param);
        void visit(StringParameter *param);
        void visit(ConstantParameter *param);
        void visit(ReturnParameter *param);
        void visit(InlineLoadParameter *param);
        void visit_enter(CallDecl *param);
        void visit_exit(CallDecl *param);
        void visit_enter(CodeDecl *param);
        void visit_exit(CodeDecl *param);
        void visit(FunctionDataDecl *param);
        void visit(SymbolDataDecl *param);
        void visit(DataDecl *param);
        void visit_enter(RopScript *param);
        void visit_exit(RopScript *param);

	private:
		VisitablePtr m_ast;
		TargetPtr m_target;
		
		std::shared_ptr<SymbolVisitor> m_symbol_visitor;
		std::map<std::string, unsigned int> m_functions, m_symbols;
		
};

#endif // _COMPILER_H_
