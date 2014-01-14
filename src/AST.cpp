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
            p->traverse(visitor);
        });
    }
};

void StringParameter::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void ConstantParameter::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void ReturnParameter::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void InlineLoadParameter::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit(this);
}

void CallDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit_enter(this);
    
    // pass on traversal to parameters
    traverse_tree(parameters(), visitor);
    
    // notify visitor
    visitor->visit_exit(this);
}

void CodeDecl::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit_enter(this);
    
    // pass on traversal to calls
    traverse_tree(calls(), visitor);
    
    // notify visitor
    visitor->visit_exit(this);
}

template<>
void FunctionDataDecl::traverse(ASTVisitor *visitor)
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
}

void RopScript::traverse(ASTVisitor *visitor)
{
    // notify the visitor
    visitor->visit_enter(this);
    
    // pass on traversal of data & code elements
    traverse_tree(data(), visitor);
    traverse_tree(code(), visitor);
    
    // notify visitor
    visitor->visit_exit(this);
}
