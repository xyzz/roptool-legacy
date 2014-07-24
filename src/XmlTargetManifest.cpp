// roptool
#include "XmlTargetManifest.h"
#include "XmlActionVisitor.h"

// std
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <map>
#include <stdexcept>

// boost
#include <boost/lexical_cast.hpp>

// tinyxml2
#include <tinyxml2.h>

bool XmlTargetManifest::parse(const std::string& file)
{
    // create new XML document
    m_xmldoc.reset(new tinyxml2::XMLDocument);
    
    // load XML file
    if (m_xmldoc->LoadFile(file.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        // \TODO: better error message
        // error reading file
        throw std::runtime_error(std::string("Could not open manifest file: ") + file);
    }
    
    // use visitor through XML
    m_xmldoc->Accept(m_visitor.get());
    return true;
}

void XmlTargetManifest::set_version(const std::string& version)
{
    std::cout << "setting version: \"" << version << "\"\n";
}

void XmlTargetManifest::set_arch_bitlen(const std::string& bitlen)
{
    int bits;
    std::cout << "setting architecture bitlen: \"" << bitlen << "\"\n";
    
    // get integer value
    try
    {
        bits = boost::lexical_cast<int>(bitlen);
    }
    
    catch (const boost::bad_lexical_cast&)
    {
        // this is a big error, throw exception
        // \TODO: error handling
        throw std::runtime_error("Error, Architecture bitlen in target is invalid integer.");
    }
    
    // check different values
    switch (bits)
    {
        case 8:
            m_bitlen = BYTE_LENGTH;
            break;
            
        case 16:
            m_bitlen = WORD_LENGTH;
            break;
            
        case 32:
            m_bitlen = DWORD_LENGTH;
            break;
            
        case 64:
            m_bitlen = QWORD_LENGTH;
            break;
            
        default:
            throw std::runtime_error("Error, invalid architecture bit length! Valid values are 8, 16, 32 or 64.");
    }
}

void XmlTargetManifest::set_stack_alignment(const std::string& stack_alignment)
{
    std::cout << "got stack alignment: " << stack_alignment << "\n";
    
    // get integer value
    try
    {
        m_stack_alignment = std::stoul(stack_alignment, nullptr, 0);
    }
    
    catch (const std::invalid_argument&)
    {
        // this is a big error, throw exception
        // \TODO: error handling
        throw std::runtime_error("Error, stack alignment is an invalid integer");
    }
}

void XmlTargetManifest::set_caller_gadget(const std::string& caller_gadget)
{
    std::cout << "got caller gadget: " << caller_gadget << "\n";
    m_caller_gadget = caller_gadget;
}
   
XmlTargetManifest::XmlTargetManifest(void)
{
    using namespace std::placeholders;
    
    // create visitor
    m_visitor.reset(new XmlActionVisitor());
    
    // add handlers
    VisitorAction set_version = std::bind(&XmlTargetManifest::set_version, this, _1);
    VisitorAction set_arch_bitlen = std::bind(&XmlTargetManifest::set_arch_bitlen, this, _1);
    VisitorAction set_stack_alignment = std::bind(&XmlTargetManifest::set_stack_alignment, this, _1);
    VisitorAction set_caller_gadget = std::bind(&XmlTargetManifest::set_caller_gadget, this, _1);
    m_visitor->addHandler("version", set_version, true);
    m_visitor->addHandler("arch_bitlen", set_arch_bitlen, true);
    m_visitor->addHandler("stack_alignment", set_stack_alignment, true);
    m_visitor->addHandler("caller_gadget", set_caller_gadget, true);
}

XmlTargetManifest::~XmlTargetManifest(void)
{

}
