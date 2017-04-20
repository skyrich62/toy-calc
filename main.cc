#include <iostream>
#include <iomanip>
#include "grammar.h"
#include "tao/pegtl/analyze.hpp"
#include "tao/pegtl/tracer.hpp"
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

template<typename Rule> struct actions : tao::pegtl::nothing<Rule> { };

template<typename Expected>
struct missing
{
    static const std::string expected_message;

    template<typename Input>
    static void apply(const Input &in)
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
ERROR(grammar::RPAR, "a right parenthesis");

using rpar_or_semi = tao::pegtl::sor<grammar::RPAR, grammar::SEMI>;
ERROR(rpar_or_semi, "a right paren")

template<typename Rule>
struct recognizer
{
    template <typename Input>
    static void apply(Input &in)
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

RECOGNIZER(grammar::expression)
RECOGNIZER(grammar::term)
RECOGNIZER(grammar::factor)
RECOGNIZER(grammar::unary_adding_operator)
RECOGNIZER(grammar::binary_adding_operator)
RECOGNIZER(grammar::binary_multiplying_operator)
RECOGNIZER(grammar::SEMI)
RECOGNIZER(grammar::statement)
RECOGNIZER(grammar::identifier)
RECOGNIZER(grammar::number)

int main(int argc, char *argv[])
{
   struct grammar: tao::pegtl::must<
                                 tao::pegtl::plus<::grammar::statement>,
                                 tao::pegtl::eof
                               > { };

#ifdef TRACE
#define controls tracer
#else
#define controls tao::pegtl::normal
#endif


   if (argc == 1) {
       const size_t issues_found = tao::pegtl::analyze<grammar>();
       std::cout << "Grammar analyzed, found "
                 << issues_found
                 << " issues."
                 << std::endl;
       return 0;
   }
   for (decltype(argc) i = 1; i < argc; ++i) {
       try {
           tao::pegtl::argv_input<> file(argv, i, argv[i]);
           //tao::pegtl::file_parser parser(file);
           //parser.parse<grammar, actions, controls>();
           auto res = tao::pegtl::parse<grammar, actions, controls>(file);
           if (res && (parse_errors == 0u)) {
             std::cout << "Good parse" << std::endl;
           } else {
             std::cout << "Bad parse, "
                       << parse_errors
                       << " errors found."
                       << std::endl;
           }
       } catch (const tao::pegtl::parse_error &e) {
           std::cout << e.what() << std::endl;
       } catch (const tao::pegtl::input_error &e) {
           std::cout << e.what() << std::endl;
       }
   }
   return 0;
}
