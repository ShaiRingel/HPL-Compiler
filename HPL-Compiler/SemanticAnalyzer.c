#include "SemanticAnalyzer.h"
#include <stdlib.h>
#include <stdio.h>

void handleExpressions(SemanticAnalyzer* analyzer, CSTNode* node);

SymbolData lookupSymbolOrError(SemanticAnalyzer* analyzer, char* lexeme) {
    SymbolData data = lookupSymbol(analyzer->currentScope, lexeme);

    if (data.type == TYPE_NONE) {
        printf(RED "Semantic Error: Undeclared identifier '%s'.\n" RESET, lexeme);
        exit(EXIT_FAILURE);
    }
    return data;
}

SemanticAnalyzer* initSemanticAnalyzer() {
    SemanticAnalyzer* analyzer = (SemanticAnalyzer*)malloc(sizeof(SemanticAnalyzer));
    if (!analyzer) {
        printf(RED "Error: Failed to allocate Semantic Analyzer\n" RESET);
        exit(EXIT_FAILURE);
    }

    // Create the global scope (it has no parent)
    analyzer->rootScope = createScope(NULL);

    // Start analysis in the global scope
    analyzer->currentScope = analyzer->rootScope;

    return analyzer;
}

void freeSemanticAnalyzer(SemanticAnalyzer* analyzer) {
    freeScopeTree(analyzer->rootScope);
    free(analyzer);
}

void enterScope(SemanticAnalyzer* analyzer) {
    ScopeNode* newScope = createScope(analyzer->currentScope);
    addChildScope(analyzer->currentScope, newScope);
    analyzer->currentScope = newScope;
}

void exitScope(SemanticAnalyzer* analyzer) {
    if (analyzer->currentScope->parent)
        analyzer->currentScope = analyzer->currentScope->parent;
}

void addIdentifier(SemanticAnalyzer* analyzer, char* lexeme, VarType type) {
    SymbolData data = getSymbol(analyzer->currentScope->symbols, lexeme);

    // If it exists in the *current* local scope, it's a redeclaration error
    if (data.type || (data.type == TYPE_VAR && data.value != 0)) {
        printf(RED "Semantic Error: Variable '%s' is already declared in this scope.\n" RESET, lexeme);
        exit(EXIT_FAILURE);
    }

    data.type = type;
    data.value = 1;

    putSymbol(analyzer->currentScope->symbols, lexeme, data);
    printf(GREEN "Semantic: Declared '%s' in current scope.\n" RESET, lexeme);
}

void handleVarDecl(SemanticAnalyzer* analyzer, CSTNode* node) {
    // Variable declaration: LET -> IDENT -> BE -> TYPE -> .
    CSTNode* identNode = node->firstChild->nextSibling;
    addIdentifier(analyzer, identNode->token->lexeme, TYPE_VAR);
}

void handleFuncDecl(SemanticAnalyzer* analyzer, CSTNode* node) {
    // FUNC_DECL -> To id with PARAM_LIST : BLOCK
    CSTNode* identNode = node->firstChild->nextSibling;
    addIdentifier(analyzer, identNode->token->lexeme, TYPE_FUNC);

    enterScope(analyzer);

    CSTNode* paramsNode = identNode->nextSibling->nextSibling;
    if (paramsNode->symbol == NON_TERMINAL_PARAM_LIST) {
        // Simple traversal for parameters
        CSTNode* curr = paramsNode->firstChild;
        while (curr) {
            if (curr->token && curr->token->type == TOKEN_IDENT) {
                addIdentifier(analyzer, curr->token->lexeme, TYPE_PARAM);
            }
            curr = (curr->firstChild) ? curr->firstChild : curr->nextSibling;
        }
    }

    // Find and analyze the function body BLOCK
    CSTNode* block = node->firstChild;
    while (block && block->symbol != NON_TERMINAL_BLOCK) block = block->nextSibling;
    if (block) analyzeNode(analyzer, block);

    exitScope(analyzer);
}

void handleLVal(SemanticAnalyzer* analyzer, CSTNode* node) {
    CSTNode* identNode = node->firstChild;
    SymbolData data = lookupSymbolOrError(analyzer, identNode->token->lexeme);

    // Allow both VAR and PARAM to be used as L-Values (assignments/reads)
    if (data.type != TYPE_VAR && data.type != TYPE_PARAM) {
        printf(RED "Semantic Error: '%s' is a function and cannot be used as a variable.\n" RESET, identNode->token->lexeme);
        exit(EXIT_FAILURE);
    }

    if (identNode->nextSibling && identNode->nextSibling->token->type == TOKEN_ATPOSITION) {
        handleExpressions(analyzer, identNode->nextSibling->nextSibling->nextSibling);
    }
}

void handleFuncCallStmt(SemanticAnalyzer* analyzer, CSTNode* node) {
    CSTNode* identNode = (node->symbol == NON_TERMINAL_FUNC_CALL) ? node->firstChild : node->firstChild->firstChild;
    SymbolData data = lookupSymbolOrError(analyzer, identNode->token->lexeme);

    if (data.type != TYPE_FUNC) {
        printf(RED "Semantic Error: '%s' is not a function.\n" RESET, identNode->token->lexeme);
        exit(EXIT_FAILURE);
    }

    // Analyze arguments if present
    CSTNode* argsNode = identNode->nextSibling;
    while (argsNode && argsNode->symbol != NON_TERMINAL_ARG_LIST) argsNode = argsNode->nextSibling;
    if (argsNode) {
        handleExpressions(analyzer, argsNode);
    }
}

void handleExpressions(SemanticAnalyzer* analyzer, CSTNode* node) {
    if (!node) return;

    if (node->symbol == NON_TERMINAL_LVAL) {
        handleLVal(analyzer, node);
        return;
    }
    else if (node->symbol == NON_TERMINAL_FUNC_CALL) {
        handleFuncCallStmt(analyzer, node);
        return;
    }

    // Traverse children for nested expressions
    CSTNode* child = node->firstChild;
    while (child) {
        handleExpressions(analyzer, child);
        child = child->nextSibling;
    }
}

void handleConditional(SemanticAnalyzer* analyzer, CSTNode* node) {
    handleExpressions(analyzer, node);
}

void handleAssignment(SemanticAnalyzer* analyzer, CSTNode* node) {
    // ASSIGN_STMT -> Set/Increase/Decrease/Multiply/Divide L_VAL (by/to) E .
    CSTNode* lvalNode = node->firstChild->nextSibling;
    handleLVal(analyzer, lvalNode);

    CSTNode* exprNode = lvalNode->nextSibling->nextSibling;
    handleExpressions(analyzer, exprNode);
}

void handleIO(SemanticAnalyzer* analyzer, CSTNode* node) {
    if (node->firstChild->token && node->firstChild->token->type == TOKEN_SHOW) {
        handleExpressions(analyzer, node->firstChild->nextSibling);
    }
    else if (node->firstChild->token && node->firstChild->token->type == TOKEN_GET) {
        // Get TYPE ask TEXT_LIT storeInto L_VAL .
        CSTNode* lval = node->firstChild;
        while (lval && lval->symbol != NON_TERMINAL_LVAL) lval = lval->nextSibling;
        if (lval) handleLVal(analyzer, lval);
    }
}

void handleLoops(SemanticAnalyzer* analyzer, CSTNode* node) {
    if (node->token && node->token->type == TOKEN_FOREACH) {
        // Foreach id in id
        lookupSymbolOrError(analyzer, node->nextSibling->token->lexeme);
        lookupSymbolOrError(analyzer, node->nextSibling->nextSibling->nextSibling->token->lexeme);
    }
    else {
        handleExpressions(analyzer, node->nextSibling); // E or COND
    }
}

void handleBlock(SemanticAnalyzer* analyzer, CSTNode** node) {
    CSTNode* block = (*node)->firstChild;
    while (block && block->symbol != NON_TERMINAL_BLOCK) block = block->nextSibling;
    analyzeNode(analyzer, block);
}

void handleCtrlFlow(SemanticAnalyzer* analyzer, CSTNode* node) {
    CSTNode* child = node->firstChild;
    if (!child) return;

    if (child->symbol == NON_TERMINAL_IF_STMT) {
        // IF_STMT -> If COND then : BLOCK ELIF_LIST
        handleConditional(analyzer, child->firstChild->nextSibling); // COND

        handleBlock(analyzer, &child);

        // Handle ELIF/ELSE (recursive via analyzeNode)
        if (child) analyzeNode(analyzer, child->nextSibling);
    }
    else if (child->symbol == NON_TERMINAL_LOOP_STMT) {
        // LOOP_STMT -> Repeat E iterations : BLOCK | While COND : BLOCK | Foreach id in id : BLOCK
        handleLoops(analyzer, child->firstChild);

        handleBlock(analyzer, &child);
    }
}

void handleFuncRet(SemanticAnalyzer* analyzer, CSTNode* node) {
    // FUNC_RET -> Return E .
    CSTNode* exprNode = node->firstChild->nextSibling;
    handleExpressions(analyzer, exprNode);
}

void analyzeNode(SemanticAnalyzer* analyzer, CSTNode* node) {
    if (!node) return;

    switch (node->symbol) {
    case NON_TERMINAL_BLOCK: {
        CSTNode* bChild;
        enterScope(analyzer);
        bChild = node->firstChild;
        while (bChild) { analyzeNode(analyzer, bChild); bChild = bChild->nextSibling; }
        exitScope(analyzer);
        return;
    }
    case NON_TERMINAL_VAR_DECL:    handleVarDecl(analyzer, node); break;
    case NON_TERMINAL_FUNC_DECL:   handleFuncDecl(analyzer, node); return;
    case NON_TERMINAL_ASSIGN_STMT: handleAssignment(analyzer, node); break;
    case NON_TERMINAL_IO_STMT:     handleIO(analyzer, node); break;
    case NON_TERMINAL_CTRL_FLOW:   handleCtrlFlow(analyzer, node); break;
    case NON_TERMINAL_FUNC_CALL_STMT: handleFuncCallStmt(analyzer, node); break;
    case NON_TERMINAL_FUNC_RET:    handleFuncRet(analyzer, node); break;
    default: {
        CSTNode* child = node->firstChild;
        while (child) {
            analyzeNode(analyzer, child);
            child = child->nextSibling;
        }
        break;
    }
    }
}

void analyzeAST(SemanticAnalyzer* analyzer, CSTNode* root) {
    if (!root) return;
    printf(CYAN "--- Starting Semantic Analysis ---\n" RESET);
    analyzeNode(analyzer, root);
    printf(GREEN "--- Semantic Analysis Complete: No errors found ---\n" RESET);
}