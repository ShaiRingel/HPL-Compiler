#pragma once

#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

typedef enum {
    False,
    True
} Bool;

typedef enum {
    TOKEN_EOF = -1,        // End of file/input
    TOKEN_NEWLINE,         // Newline character
    TOKEN_NUMBER,          // Numeric literal (integer/decimal)
    TOKEN_IDENT,           // Identifier (variable/function name)
    TOKEN_TEXT,            // String literal

    // Keywords (start at 101)
    TOKEN_LET = 101,       // "Let" → variable declaration
    TOKEN_BE,              // "be" → type binding in declaration
    TOKEN_SET,             // "Set" → assignment
    TOKEN_TO,              // "to" → assignment target
    TOKEN_BY,              // "by" → used in Increase/Decrease
    TOKEN_SHOW,            // "Show" → output/print
    TOKEN_INTEGER,         // "integer" → integer type
    TOKEN_REAL,            // "real" → real type
    TOKEN_INCREASE,        // "Increase" → += operator
    TOKEN_DECREASE,        // "Decrease" → -= operator
    TOKEN_MULTIPLY,        // "Multiply" → *= operator
    TOKEN_DIVIDE,          // "Divide" → /= operator
    TOKEN_RETURN,          // "Return" → return from function
    TOKEN_OTHERWISE,       // "Otherwise" → else/else if
    TOKEN_IF,              // "If" → conditional
    TOKEN_THEN,            // "then" → conditional body
    TOKEN_REPEAT,          // "Repeat" → for loop
    TOKEN_ITERATIONS,           // "times" → repeat count
    TOKEN_FOREACH,         // "Foreach" → foreach loop
    TOKEN_IN,              // "in" → foreach collection
    TOKEN_WHILE,           // "While" → while loop
    TOKEN_GET,             // "Get" → input
    TOKEN_ASK,             // "ask" → input prompt
    TOKEN_STOREINTO,       // "storeInto" → input target
    TOKEN_TOFUNC,          // "To" → function definition
    TOKEN_WITH,            // "with" → function parameter
    TOKEN_ATPOSITION,       // "atPosition" → array indexing

    // Operators (start at 201)
    TOKEN_ADD,             // '+' → addition
    TOKEN_SUB,             // '-' → subtraction
    TOKEN_MUL,             // '*' → multiplication
    TOKEN_DIV,             // '/' → division
    TOKEN_EQTO,            // '==' → equalsTo
    TOKEN_NOTEQ,           // '!=' → notEqualTo
    TOKEN_LT,              // '<' → lessThan
    TOKEN_ATMOST,          // '<=' → atMost
    TOKEN_GT,              // '>' → greaterThan
    TOKEN_ATLEAST,         // '>=' → atLeast
    TOKEN_REMAINDEROF,     // "remainderOf" keyword
    TOKEN_DIVIDEDBY,       // "dividedBy" keyword
    TOKEN_AND,             // "and" → logical AND
    TOKEN_OR,              // "or" → logical OR
    TOKEN_NOT,             // "not" → logical NOT

    // Special Symbols (start at 301)
    TOKEN_LPAREN = 301,    // '(' → left parenthesis
    TOKEN_RPAREN,          // ')' → right parenthesis
    TOKEN_PLUS,            // '+' → string concatenation
    TOKEN_COLON,           // ':' → block start (loops, if)
    TOKEN_DOT,             // '.' → end of statement
} TokenType;