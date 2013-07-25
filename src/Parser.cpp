
#include "Parser.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
namespace classic = boost::spirit::classic;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
        
template <typename Iterator>
struct ropscript_grammar : qi::grammar<Iterator, std::string()>
{
    ropscript_grammar() : ropscript_grammar::base_type(simple2, "simple2")
    {
        // define what is classed as an identifier
        // this will include function call names, variable names
        // symbol names and so forth
        
        simple2 = identifier > '=' > "rainbow";
        
        identifier =  qi::lexeme[letter >> *(letter | decimal_digit)];
        decimal = qi::int_;
        hex = (qi::lit("0x") | qi::lit("0X")) >> qi::hex;
        octal = qi::lit("0") >> qi::oct;
        number = hex | octal | decimal;
        
        // basic rules define what is a letter and what is a number
        letter = qi::char_("a-zA-Z_");
        decimal_digit = qi::char_("0-9");
        
        identifier.name("identifier");  
        letter.name("letter");
        decimal_digit.name("decimal_digit");
        simple2.name("simple2");
    }
    
    qi::rule<Iterator, char()> decimal_digit;
    qi::rule<Iterator, char()> letter;
    qi::rule<Iterator, int()> decimal;
    qi::rule<Iterator, int()> octal;
    qi::rule<Iterator, int()> hex;
    qi::rule<Iterator, int()> number;
    qi::rule<Iterator, std::string()> identifier;
    qi::rule<Iterator, std::string()> simple2;
};

bool parse(const char *filename)
{
    std::ifstream ifs;
    std::string out;
    
    // open file
    ifs.open(filename, std::ios_base::in);
    
    // check if open failure
    if (!ifs)
    {
        std::cerr << "Error: Could not open input file: " << filename << std::endl;
        return false;
    }
        
    typedef std::istreambuf_iterator<char> base_iterator_type;
    base_iterator_type in_begin(ifs);

    // convert input iterator to forward iterator, usable by spirit parser
    typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
    forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
    forward_iterator_type fwd_end;

    // wrap forward iterator with position iterator, to record the position
    typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
    pos_iterator_type position_begin(fwd_begin, fwd_end, filename);
    pos_iterator_type position_end;
  
    ropscript_grammar<pos_iterator_type> parser;
    bool r = false;
    
    qi::rule<pos_iterator_type> line_comment = (qi::lit("//") >> *(ascii::char_ - qi::eol) >> qi::eol);
    qi::rule<pos_iterator_type> block_comment = (qi::lit("/*") >> *(ascii::char_ - '*') >> +qi::lit('*') >> '/');
    
    try
    {
        // parse the script file
        r = qi::phrase_parse(position_begin, position_end, parser, ascii::space | line_comment | block_comment, out);
    }

    // catch input expectation failure
    catch (const qi::expectation_failure<pos_iterator_type>& e)
    {
        std::stringstream msg;
        
        // get the position of the iterator relative to the file
        const classic::file_position_base<std::string>& pos = e.first.get_position();
       
        // construct error message for the exception
        msg << "Parse error in file \"" << pos.file << "\""
            << ", line " << pos.line << ", column " << pos.column << std::endl 
            << "Expected: " << e.what_;
            
        // throw exception
        throw std::runtime_error(msg.str());
    }

    std::cout << "out: " << out << "\n";
    return r;
}
