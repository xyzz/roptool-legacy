#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "AST.h"

class Compiler : public ASTVisitor
{
	public:
		// CTOR
		Compiler(void) { }
		Compiler(VisitablePtr ast) { setAST(ast); }
		
		// set ast
		void setAST(VisitablePtr ast) { m_ast = ast; }
		
		// visitor methods
		void visit(CallParameter *param);
        void visit(CallDecl *param);
        void visit(CodeDecl *param);
        void visit(FunctionDataDecl *param);
        void visit(SymbolDataDecl *param);
        void visit(DataDecl *param);
        void visit(RopScript *param);

	private:
		VisitablePtr m_ast;
};

#endif // _COMPILER_H_
