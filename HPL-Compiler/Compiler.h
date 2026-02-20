#pragma once
#include "Lexer.h"

typedef struct {
	Lexer *lexer;
	// Parser *parser;
	// SemanticAnalyzer *sa;
	// CodeGenerator *cg;
} Compiler;

// Initialize Components
Compiler* initCompiler(char* filePath);
// Starts the compilation process
Compiler* startCompiler(Compiler* compiler);