#pragma once
#include "Lexer.h"

typedef struct {
	Lexer lexer;
	// Parser parser;
	// SemanticAnalyzer sa;
	// CodeGenerator cg;
} Compiler;

//Initialize Components
Compiler* initCompiler();