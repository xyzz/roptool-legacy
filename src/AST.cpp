#include "AST.h"

// std
#include <algorithm>

namespace 
{
    template<typename T>
    void traverse_tree(std::vector<T> ast_tree, ASTVisitor *visitor)
    {
        // pass on traversal of data element
        std::for_each(ast_tree.begin(), ast_tree.end(), [=](T& p)
        {
            p.traverse(visitor);
        });
    }
};

void CallParameter::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void CallDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
    
    // pass on traversal to parameters
    traverse_tree(parameters(), visitor);
}

void CodeDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
    
    // pass on traversal to calls
    traverse_tree(calls(), visitor);
}

template<>
void FunctionDataDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void SymbolDataDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void DataDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
    
    // pass on traversal to functions
    traverse_tree(functions(), visitor);
    
    // pass on traversal to symbols
    traverse_tree(symbols(), visitor);
}

void RopScript::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
    
    // pass on traversal of data & code elements
    traverse_tree(data(), visitor);
    traverse_tree(code(), visitor);
}
