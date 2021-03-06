/**
 * @file
 * This file describes the grammar of the toy-calc project.
 * @copyright
 * (C) 2017 Rich Newman.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
struct identifier_next  : ranges<'a', 'z', 'A', 'Z', '0', '9', '_'> { };

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

struct expression : state<states::binary_operation,
                      sor<
                        state<states::prefix_unary_operation,
                          unary_adding_operator,
                          term
                        >,
                        term
                      >,
                      star<
                        binary_adding_operator,
                        recover<term, success>
                      >
                    > { };

struct term : state<states::binary_operation,
                recover_list<factor, binary_multiplying_operator, success>
              > { };

struct parend_expr : seq<
                       LPAR,
                       state<states::parend_expr, expression>,
                       recover<RPAR, success>
                     > { };

struct factor : sor<
                  parend_expr,
                  number,
                  identifier
                > { };


struct statement : state<states::statement,
                     recover<expression, ignore>,
                     recover<SEMI, eolf>
                   > { };

struct compilation :seq<
                      plus<statement>,
                      eof
                    > { };

} // namespace grammar

#endif // GRAMMAR_H__ }
