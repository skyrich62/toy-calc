# toy-calc
A toy calculator parsing project using the PEGTL.

This is an experiment in error recovery techniques with the PEGTL.

The basic notion is the classes:
   recover, expected and missing in the "grammar" namespace, (grammar.h)

Consider the following PEG

```
expression <- unary_adding_operator? term (binary_adding_operator term)*
```

If there is a binary_adding_operator, then there must be a term, so if
it's not there, we want to recover from that, and act like we saw the
term anyway.
