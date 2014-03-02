// roptool
#include "Compiler.h"
#include "DataSection.h"
#include "RopFunctionCall.h"

// std
#include <iostream>
#include <sstream>


u64 Compiler::bit_mask(int bits)
{
    u64 mask = 0xFFFFFFFFFFFFFFFFUL;
    return mask >> (64 - bits); 
}

void Compiler::visit(StringParameter *param)
{
    int param_bits = param->bitlen();
    int arch_bits = m_target->manifest()->arch_bitlen();
    
    // add string to data section
    DataRefPtr ref = m_data_section.add(param->value());
    
    // store
    int param_n = param_bits / arch_bits;
    
    // if there is more than one param we need to do some dummy params
    if (param_n > 1)
    {
        // align to register
        if (m_param.size() % 2) param_n++;
        
        for (int i = 0; i < (param_n-1); i++)
        {
            m_param_type.push_back('v');
            m_param.push_back(m_zero_ref);
        }
    }
    
    // record the reference id
    m_param_type.push_back('v');
    m_param.push_back(ref);
}

void Compiler::visit(ConstantParameter *param)
{
    std::list<DataRefPtr> refs;
    int param_bits = param->bitlen();
    int arch_bits = m_target->manifest()->arch_bitlen();
    
    //     get number of parameters
    int param_n = param_bits / arch_bits;
    
    // if there is more than one param we may need some dummy params
    if ((param_n > 1) && (m_param.size() % 2))
    {
        // align to register
        m_param_type.push_back('v');
        m_param.push_back(m_zero_ref);
    }
    
    // create a list
    for (int i = 0; i < param_n; i++)
    {
        DataRefPtr ref = m_data_section.add((param->value() >> ((param_n-i-1) * arch_bits)) & bit_mask(arch_bits));
        
        // add reference to list
        m_param_type.push_back('v');
        m_param.push_back(ref);
    }
}

void Compiler::visit(ReturnParameter *param)
{
    int param_bits = param->bitlen();
    int arch_bits = m_target->manifest()->arch_bitlen();
    
    // store
    int param_n = param_bits / arch_bits;
    
    // if there is more than one param we need to do some dummy params
    if (param_n > 1)
    {
        // align to register
        if (m_param.size() % 2) param_n++;
        
        for (int i = 0; i < (param_n-1); i++)
        {
            m_param_type.push_back('v');
            m_param.push_back(m_zero_ref);
        }
    }
    
    // no data needed, so pass zero.
    // use param type 'r'
    m_param_type.push_back('r');
    m_param.push_back(m_zero_ref);
}

void Compiler::visit(InlineLoadParameter *param)
{
    int param_bits = param->bitlen();
    int arch_bits = m_target->manifest()->arch_bitlen();
    
    // get constant ref
    DataRefPtr ref = m_data_section.add(param->value());
    
    // store
    int param_n = param_bits / arch_bits;
    
    // if there is more than one param we need to do some dummy params
    if (param_n > 1)
    {
        // align to register
        if (m_param.size() % 2) param_n++;
        
        for (int i = 0; i < (param_n-1); i++)
        {
            m_param_type.push_back('v');
            m_param.push_back(m_zero_ref);
        }
    }
    
    // use param type 'l'
    m_param_type.push_back('l');
    m_param.push_back(ref);
}

void Compiler::visit_enter(CallDecl *param)
{
    m_param_type.clear();
    m_param.clear();
    
    // check function name is resolvable
    if (m_functions.find(param->name()) == m_functions.end())
    {
        // name not found!
        throw std::runtime_error("Name not found!");
    }
}

void Compiler::visit_exit(CallDecl *param)
{
    // add a null terminator and construct string
    m_param_type.push_back('\0');
    
    std::string param_type_list = std::string(m_param_type.data());
    std::string call_prototype = param->name() + std::string("(") + param_type_list + ")";
    std::cout << call_prototype << "\n";
    
    // lookup best map
    GadgetMapPtr map = m_target->bestGadgetMap(call_prototype);
    
    // check for null
    if (!map)
    {
        // no map found!
        throw std::runtime_error("Target has no support for call scheme: " + param_type_list);
    }
    
    RopFunctionCallPtr function_call(new RopFunctionCall);
    
    // set function call data
    function_call->setMap(map);
    function_call->setFunction(m_functions.find(param->name())->second);
    function_call->setParameters(m_param);
    
    std::cout << "map size: " << map->size() << "\n";
    
    // set function address
   // map->setFunction(m_functions.find(param->name())->second);
    
    // add the parameters
    //map->setParameters(m_param);
    
    m_data_section.setBase(0x12345678);
    
    // add to function
    //m_function.add(function_call);
}

void Compiler::visit_enter(CodeDecl *param)
{
    std::cout << "code:" << param->name() << "\n";
    //m_function = m_code_section.create(param->name());
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
    // get a reference to zero
    m_zero_ref = m_data_section.add(0);
    
    m_target = target;
    ast->traverse(this);
}
