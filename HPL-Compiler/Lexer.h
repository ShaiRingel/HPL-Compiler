#pragma once
#define END_OF_LINE '.'
#define COMMENT "NOTE:"
#include "FileReader.h"

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

    // Operators (start at 201)
    TOKEN_EQ = 201,        // '=' → assignment
    TOKEN_ADD,             // '+' → addition
    TOKEN_SUB,             // '-' → subtraction
    TOKEN_MUL,             // '*' → multiplication
    TOKEN_DIV,             // '/' → division
    TOKEN_EQEQ,            // '==' → equalsTo
    TOKEN_NOTEQ,           // '!=' → notEqualsTo
    TOKEN_LT,              // '<' → lessThan
    TOKEN_LTEQ,            // '<=' → atMost
    TOKEN_GT,              // '>' → greaterThan
    TOKEN_GTEQ,            // '>=' → atLeast
    TOKEN_ATLEAST,         // "atLeast" keyword
    TOKEN_ATMOST,          // "atMost" keyword
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

    // Arrays
    TOKEN_ATPOSITION       // "atPosition" → array indexing
} TokenType;


typedef struct {
    const char* keyword;
    TokenType type;
} KeywordEntry;

static KeywordEntry keywordTable[] = {
    // Variable declarations
    {"Let", TOKEN_LET},
    {"be", TOKEN_BE},
    {"integer", TOKEN_INTEGER},
    {"decimal", TOKEN_REAL},

    // Assignments
    {"Set", TOKEN_SET},
    {"to", TOKEN_TO},
    {"Increase", TOKEN_INCREASE},
    {"Decrease", TOKEN_DECREASE},
    {"Multiply", TOKEN_MULTIPLY},
    {"Divide", TOKEN_DIVIDE},
    {"by", TOKEN_BY},

    // Output
    {"Show", TOKEN_SHOW},

    // Conditionals
    {"If", TOKEN_IF},
    {"then", TOKEN_THEN},
    {"Otherwise", TOKEN_OTHERWISE},

    // Loops
    {"Repeat", TOKEN_REPEAT},
    {"iterations", TOKEN_ITERATIONS},
    {"Foreach", TOKEN_FOREACH},
    {"in", TOKEN_IN},
    {"While", TOKEN_WHILE},

    // Input
    {"Get", TOKEN_GET},
    {"ask", TOKEN_ASK},
    {"storeInto", TOKEN_STOREINTO},

    // Functions
    {"To", TOKEN_TOFUNC},
    {"with", TOKEN_WITH},
    {"Return", TOKEN_RETURN},

    // Operators (word forms)
    {"equalsTo", TOKEN_EQEQ},
    {"notEqualsTo", TOKEN_NOTEQ},
    {"greaterThan", TOKEN_GT},
    {"lessThan", TOKEN_LT},
    {"atLeast", TOKEN_ATLEAST},
    {"atMost", TOKEN_ATMOST},
    {"remainderOf", TOKEN_REMAINDEROF},
    {"dividedBy", TOKEN_DIVIDEDBY},
    {"add", TOKEN_ADD},
    {"subtract", TOKEN_SUB},
    {"times", TOKEN_MUL},
    {"divide", TOKEN_DIV},
    {"and", TOKEN_AND},
    {"or", TOKEN_OR},
    {"not", TOKEN_NOT},

    // Arrays
    {"atPosition", TOKEN_ATPOSITION},
};


typedef struct {
	TokenType type;
	char* lexeme;
} Token;


typedef struct {
	FileDetails fileDetails;
	char currentChar;
	int currentRow;
	int currentCol;
} Lexer;


// Initialize the lexer
Lexer* initLexer(FileDetails fileDetails);
// Read the next non-WhiteSpace character
void nextChar(Lexer* lexer);
// Skip the WhiteSpace characters
void skipWhiteSpaces(Lexer *lexer);
// Skip the Comments in the code
void skipComments(Lexer* lexer);
// Collects a full lexeme from the lexer until the condition returns false
char* getFullLexeme(Lexer* lexer, int (*condition)(int));
// Turn character / string into corresponding token
Token tokenize(Lexer *lexer);

// End of token condition methods
int numberCondition(int c);
int wordCondition(int c);

void freeLexer(Lexer *lexer);