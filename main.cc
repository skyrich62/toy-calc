#include <iostream>
#include <iomanip>
#include <sstream>
#include "states.h"
#include "grammar.h"
#include "pvisitor.h"
#include "rvisitor.h"

#include "tao/pegtl/analyze.hpp"
#include "tao/pegtl/argv_input.hpp"

static size_t indent = 0u;
static size_t parse_errors = 0u;

template<typename Rule> struct tracer : tao::pegtl::normal<Rule>
{
    template<typename Input, typename... States>
    static void start(const Input &in, States&&...)
    {
        std::cout << in.position()
                  << ": start   "
                  << std::setw(::indent) << " "
                  << tao::pegtl::internal::demangle<Rule>()
                  << std::endl;
        ++::indent;
    }


    template<typename Input, typename... States>
    static void success(const Input &in, States&&...)
    {
        --::indent;
        std::cout << in.position()
                  << ": success "
                  << std::setw(::indent) << " "
                  << tao::pegtl::internal::demangle<Rule>()
                  << std::endl;
    }


    template<typename Input, typename... States>
    static void failure(const Input &in, States&&...)
    {
        --::indent;
        std::cout << in.position()
                  << ": failure "
                  << std::setw(::indent) << " "
                  << tao::pegtl::internal::demangle<Rule>()
                  << std::endl;
    }
};

template<typename Rule>
struct controls : tao::pegtl::normal<Rule> { };

#ifdef TRACE
#define TRACER(Rule) template<> struct controls<Rule> : tracer<Rule> { };
#else
#define TRACER(Rule)
#endif

TRACER(grammar::expression)
TRACER(grammar::term)
TRACER(grammar::factor)
TRACER(grammar::PLUS_sym)
TRACER(grammar::MINUS_sym)
TRACER(grammar::STAR_sym)
TRACER(grammar::SLASH_sym)
TRACER(grammar::SEMI_sym)
TRACER(grammar::statement)
TRACER(grammar::identifier)
TRACER(grammar::number)
TRACER(grammar::LPAR_sym)
TRACER(grammar::RPAR_sym)

template<typename Rule> struct actions : tao::pegtl::nothing<Rule> { };

template<typename Expected>
struct missing
{
    static const std::string expected_message;

    template<typename Input, typename... States>
    static void apply(const Input &in, States&& ...st)
    {
        ++parse_errors;
        std::cout << in.position()
                  << ": expected "
                  << expected_message
                  << std::endl;
        std::cout << in.position().source
                  << std::endl
                  << std::setw(in.position().byte_in_line+1)
                  << std::right
                  << "^"
                  << std::endl;
    }
};

#define ERROR(Rule, message) \
template<> struct actions<grammar::missing<Rule>> : missing<Rule> { }; \
template<> const std::string missing<Rule>::expected_message{ message };

ERROR(grammar::term, "a term")
ERROR(grammar::factor, "a factor")
ERROR(grammar::expression, "an expression")
ERROR(grammar::SEMI, "a semicolon")
ERROR(grammar::identifier, "an identifier")
ERROR(grammar::number, "a number")
ERROR(grammar::RPAR, "a right parenthesis")
ERROR(grammar::statement, "a statement")

using rpar_or_semi = tao::pegtl::sor<grammar::RPAR, grammar::SEMI>;
ERROR(rpar_or_semi, "a right paren")

template<typename Rule>
struct recognizer
{
    template <typename Input, typename... States>
    static void apply(Input &in, States&& ...st)
    {
        if (in.string().empty()) {
            return;
        }
        std::cout << in.position() << ": "
                  << "*** Recognized a "
                  << tao::pegtl::internal::demangle<Rule>()
                  << ": " << in.string()
                  << std::endl;
    }
};

#define RECOGNIZER(Rule) \
template<> struct actions<Rule> : recognizer<Rule> { };

//RECOGNIZER(grammar::expression)
//RECOGNIZER(grammar::term)
//RECOGNIZER(grammar::factor)
//RECOGNIZER(grammar::unary_adding_operator)
//RECOGNIZER(grammar::binary_adding_operator)
//RECOGNIZER(grammar::binary_multiplying_operator)
//RECOGNIZER(grammar::SEMI)
//RECOGNIZER(grammar::statement)
//RECOGNIZER(grammar::identifier)
//RECOGNIZER(grammar::number)
//RECOGNIZER(grammar::LPAR)
//RECOGNIZER(grammar::RPAR)

template<typename Operator>
struct operation
{
    template<typename Input>
    static void apply(const Input &in, states::operation &st)
    {
        st.setOperator(in.string());
    }
};

template<> struct actions<grammar::PLUS_sym>  : operation<grammar::PLUS_sym>
{ };
template<> struct actions<grammar::STAR_sym>  : operation<grammar::STAR_sym>
{ };
template<> struct actions<grammar::MINUS_sym> : operation<grammar::MINUS_sym>
{ };
template<> struct actions<grammar::SLASH_sym> : operation<grammar::SLASH_sym>
{ };

template<> struct actions<grammar::identifier>
{
    template<typename Input>
    static void apply(const Input &in, states::operation &st)
    {
        auto node = new nodes::identifier;
        node->setIdentifier(in.string());
        st.addOperand(std::shared_ptr<nodes::node>(node));
    }
};

template<> struct actions<grammar::number>
{
    template<typename Input>
    static void apply(const Input &in, states::operation &st)
    {
        std::istringstream is(in.string());
        long long int val;
        is >> val;
        auto node = new nodes::numeric_literal;
        node->setValue(val);
        st.addOperand(std::shared_ptr<nodes::node>(node));
    }
};

int main(int argc, char *argv[])
{

   if (argc == 1) {
       const size_t issues_found = tao::pegtl::analyze<grammar::compilation>();
       std::cout << "Grammar analyzed, found "
                 << issues_found
                 << " issues."
                 << std::endl;
       return 0;
   }
   for (decltype(argc) i = 1; i < argc; ++i) {
     try {
       using compilation = grammar::compilation;
       using tao::pegtl::parse;
       using tao::pegtl::argv_input;

       argv_input<> file(argv, i, argv[i]);
       states::statement_list root;
       auto res = parse<compilation, actions, controls>(file, root);
       if (res && (parse_errors == 0u)) {
         std::cout << "Good parse" << std::endl;
       } else {
         std::cout << "Bad parse, "
                   << parse_errors
                   << " errors found."
                   << std::endl;
       }
       printing_visitor pv;
       nodes::traversal ilt(pv);

       std::cout << "----- Parse tree ------- " << std::endl;
       ilt.traverse(*(root.get()), nodes::traversal::PRE);

       std::cout << "----- Reconstruction ----" << std::endl;
       reconstructing_visitor rv;
       natural_visitor nv(rv);
       ilt.set_visitor(nv);
       ilt.traverse(*(root.get()), nodes::traversal::NATURAL);
     } catch (const tao::pegtl::parse_error &e) {
       std::cout << e.what() << std::endl;
     } catch (const tao::pegtl::input_error &e) {
       std::cout << e.what() << std::endl;
     }
   }
   return 0;
}
