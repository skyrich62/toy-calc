# toy-calc
A toy calculator parsing project using the PEGTL.

This is an experiment in error recovery techniques with the PEGTL.

The basic notion is the classes:
   recover, expected and missing in the "grammar" namespace, (grammar.h)

Consider the following PEG

```
expression ← unary_adding_operator? term (binary_adding_operator term)*
```

If there is a `binary_adding_operator`, then there must be a `term`, so if
it's not there, we want to recover from that, and act like we saw the
term anyway. So, wrap term with recover, like this:

```
struct expression : seq<
                      opt<unary_adding_operator>,
                      seq<
                        term,
                        star<
                          binary_adding_operator,
                          recover<term, success>
                        >
                      >
                    > { };
```

The recover parsing rule tries to parse a term, but if that fails, then
it will call `expected<term, success>`, which in turn will call
`missing<term>`, and `skipping<success>`.  (I'm not quite happy with these
names, this is still experimental.)

The user can add an action for `missing<R>` to supply an interesting,
(possibly helpful), error message.  Class `skipping` is used for error
recovery and can skip input until a recoverable state has been found.
This will probably have to be modified to allow for state changes and
a set of terminating rules.  For now, it will skip until the given
rule, (in this case `success`), or until end of line or end of file.

The idea being that sometimes we might want to skip until we see an
appropriate terminating token, but sometimes we just want to act like
the thing we wanted was there anyway, and skip nothing.

## Sample runs

I've added a `recognizer` class which simply decodes what was recognized
from the input.

First, a parse of a correct input:
```
$ ./main 'a;'
a;:1:0(0): *** Recognized a grammar::identifier: a
a;:1:0(0): *** Recognized a grammar::factor: a
a;:1:0(0): *** Recognized a grammar::term: a
a;:1:0(0): *** Recognized a grammar::expression: a
a;:1:1(1): *** Recognized a grammar::SEMI: ;
a;:1:0(0): *** Recognized a grammar::statement: a;
Good parse
```

Now, let's forget the semicolon:

```
./main 'a'
a:1:0(0): *** Recognized a grammar::identifier: a
a:1:0(0): *** Recognized a grammar::factor: a
a:1:0(0): *** Recognized a grammar::term: a
a:1:0(0): *** Recognized a grammar::expression: a
a:1:1(1): expected a semicolon
a:1:0(0): *** Recognized a grammar::statement: a
Bad parse
```

Now, let's forget a term:

```
./main 'a+;'
a+;:1:0(0): *** Recognized a grammar::identifier: a
a+;:1:0(0): *** Recognized a grammar::factor: a
a+;:1:0(0): *** Recognized a grammar::term: a
a+;:1:1(1): *** Recognized a grammar::binary_adding_operator: +
a+;:1:2(2): expected a term
a+;:1:0(0): *** Recognized a grammar::expression: a+
a+;:1:2(2): *** Recognized a grammar::SEMI: ;
a+;:1:0(0): *** Recognized a grammar::statement: a+;
Bad parse
```

With proper state information, (perhaps an AST), then we could correctly
identify the "statement" above as something like:

```
a+;:1:0(0): *** Recognized a grammar::statement: a+<error_missing_operand>;
```

But that's an exercise for another time. :)

## Update
The recognizer has been commented out.  Now a full parse-tree is created, and then printed out after the parse.
Just for fun, I've also added a reconstructer which reconstructs the original input.

New sample runs look like this:

```
$ ./main '(a+b) * (c-d);'
Good parse
----- Parse tree -------
0: statement_list
1: statement, expression: 6
6: binary_op:  *, operands: 2 7
2: parend_expr, expression: 4
4: binary_op:  +, operands: 3 5
3: identifier: a
5: identifier: b
7: parend_expr, expression: 9
9: binary_op:  -, operands: 8 10
8: identifier: c
10: identifier: d
----- Reconstruction ----
(a + b) * (c - d);
```

And with an error in the input stream:

```
$ ./main '(a +) *5;'
(a +) *5;:1:4(4): expected a term
(a +) *5;
    ^
Bad parse, 1 errors found.
----- Parse tree -------
0: statement_list
1: statement, expression: 6
6: binary_op:  *, operands: 2 7
2: parend_expr, expression: 4
4: binary_op:  +, operands: 3 5
3: identifier: a
5: error, children:
7: numeric_literal: 5
----- Reconstruction ----
(a + {error}) * 5;

```

Note the insertion of an "error" node in the AST.
