#ifndef _AST_H_
#define _AST_H_

// std
#include <string>
#include <memory>
#include <vector>
#include <map>

// boost
#include <boost/variant.hpp>

// forward declaration of the classes in the AST
class CallParameter;
class CallDecl;
class CodeDecl;
class DataDecl;
class RopScript;

template<typename T>
class DataDeclImpl;

typedef unsigned int Function;
typedef boost::variant<unsigned int, std::string> Symbol;


typedef DataDeclImpl<Function> FunctionDataDecl;
typedef DataDeclImpl<Symbol> SymbolDataDecl;

class ASTVisitor
{
    public:
        virtual void visit(CallParameter *param) = 0;
        virtual void visit(CallDecl *param) = 0;
        virtual void visit(CodeDecl *param) = 0;
        virtual void visit(FunctionDataDecl *param) = 0;
        virtual void visit(SymbolDataDecl *param) = 0;
        virtual void visit(DataDecl *param) = 0;
        virtual void visit(RopScript *param) = 0;
};

class ASTVisitable
{
    public:
        virtual void traverse(ASTVisitor *visitor) = 0;
};

typedef boost::variant<unsigned int, std::string> Parameter;

class CallParameter : public ASTVisitable
{
    public:
        Parameter& parameter(void) { return m_param; }
        const Parameter& parameter(void) const { return m_param; }
        void setParameter(const Parameter& param) { m_param = param; }
        
        void traverse(ASTVisitor *visitor);
    
    private:
        Parameter m_param;
};

typedef std::vector<CallParameter> CallParameterList;

class CallDecl : public ASTVisitable
{
    public:
        std::string& name(void) { return m_name; }
        const std::string& name(void) const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        
        CallParameterList& parameters(void) { return m_params; }
        const CallParameterList& parameters(void) const { return m_params; }
        void addParameter(const CallParameter& param) { m_params.push_back(param); }
        void addParameter(const CallParameterList& list) { m_params.insert(m_params.end(), list.begin(), list.end()); }
        void traverse(ASTVisitor *visitor);
    
    private:
        CallParameterList m_params;
        std::string m_name;
};

typedef std::vector<CallDecl> CallDeclList;

class CodeDecl : public ASTVisitable
{
    public:
        const std::string& name(void) const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        
        const CallDeclList& calls(void) const { return m_calls; }
        void addCall(const CallDecl& call) { m_calls.push_back(call); }
        
        void traverse(ASTVisitor *visitor);
        
    private:
        std::string m_name;
        CallDeclList m_calls;
};

template<typename T>
class DataDeclImpl : public ASTVisitable
{
    public:
        const std::string& name(void) const { return m_name; }
        void setName(std::string name) { m_name = name; }
        
        const T& getData(void) const { return m_data; }
        void setData(const T& data) { m_data = data; }
        
        void traverse(ASTVisitor *visitor);
        
    private:
        std::string m_name;
        T m_data;
};

typedef std::vector<FunctionDataDecl> FunctionDataDeclList;
typedef std::vector<SymbolDataDecl> SymbolDataDeclList;

class DataDecl : public ASTVisitable
{
    public:
        const FunctionDataDeclList& functions(void) const { return m_func_data; } 
        void addFunction(const FunctionDataDecl& func_data) { m_func_data.push_back(func_data); }
        
        const SymbolDataDeclList& symbols(void) const { return m_sym_data; }
        void addSymbol(const SymbolDataDecl& symbol_data) { m_sym_data.push_back(symbol_data); }
        
        void traverse(ASTVisitor *visitor);
        
    private:
        SymbolDataDeclList m_sym_data;
        FunctionDataDeclList m_func_data;
};

typedef std::vector<DataDecl> DataDeclList;
typedef std::vector<CodeDecl> CodeDeclList;

class RopScript : public ASTVisitable
{
    public:
        const DataDeclList& data(void) const { return m_data; }
        void addData(const DataDecl& data) { m_data.push_back(data); }
        
        const CodeDeclList& code(void) const { return m_code; }
        void addCode(const CodeDecl& code) { m_code.push_back(code); }
        
        void traverse(ASTVisitor *visitor);
        
    private:
        DataDeclList m_data;
        CodeDeclList m_code;
};

typedef std::shared_ptr<RopScript> RopScriptShared;
typedef std::shared_ptr<ASTVisitable> VisitablePtr;

#endif //_AST_H_