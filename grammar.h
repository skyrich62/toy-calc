#ifndef GRAMMAR_H__ // {
#define GRAMMAR_H__

#include "tao/pegtl.hpp"

namespace grammar {
using namespace tao::pegtl;

struct number : plus<ascii::digit> { };

struct PLUS_sym  : one<'+'> { };
struct MINUS_sym : one<'-'> { };
struct STAR_sym  : one<'*'> { };
struct SLASH_sym : one<'/'> { };
struct LPAR_sym  : one<'('> { };
struct RPAR_sym  : one<')'> { };
struct SEMI_sym  : one<';'> { };

struct ignore : ascii::space { };

struct PLUS  : pad<PLUS_sym,  ignore> { };
struct MINUS : pad<MINUS_sym, ignore> { };
struct STAR  : pad<STAR_sym,  ignore> { };
struct SLASH : pad<SLASH_sym, ignore> { };
struct LPAR  : pad<LPAR_sym,  ignore> { };
struct RPAR  : pad<RPAR_sym,  ignore> { };
struct SEMI  : pad<SEMI_sym,  ignore> { };

struct identifier_first : ranges<'a', 'z', 'A', 'Z'> { };
struct identifier_next : ranges<'a', 'z', 'A', 'Z', '0', '9', '_'> { };

struct identifier : seq<
                      identifier_first,
                      star<identifier_next>
                    > { };

struct unary_adding_operator : sor<PLUS, MINUS> { };
struct binary_adding_operator: sor<PLUS, MINUS> { };
struct binary_multiplying_operator : sor<STAR, SLASH> { };

template<typename T>
struct skipping : until< sor< T, eolf> > { };

template<typename R>
struct missing : success { };

template<typename R, typename T>
struct expected : seq<missing<R>, skipping<T>> { };

template<typename R, typename T = R>
struct recover : sor<R, expected<R, T>> { };

template<typename Rule, typename Sep, typename Terminator>
struct recover_list : seq<
                        Rule,
                        star<
                          Sep,
                          recover<Rule, Terminator>
                        >
                      > { };

struct term;
struct factor;

struct expression : seq<
                      opt<unary_adding_operator>,
                      recover_list<term, binary_adding_operator, success>
                    > { };

struct term : recover_list<factor, binary_multiplying_operator, success> { };

struct factor : sor<
                  seq<
                    LPAR,
                    expression,
                    recover<RPAR, success>
                  >,
                  number,
                  identifier
                > { };


struct statement : seq<expression, recover<SEMI, pad<success, ignore>>> { };

} // namespace grammar

#endif // GRAMMAR_H__ }
