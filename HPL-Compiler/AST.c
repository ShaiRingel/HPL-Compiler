#include "AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* createNode(ASTNodeType type) {
	ASTNode *node = (ASTNode*) malloc(sizeof(ASTNode));
	if (!node) {
		perror("Malloc failed!");
		exit(EXIT_FAILURE);
	}
	memset(node, NULL, sizeof(node));
	node->type = type;
}

void freeAST(ASTNode* node) {
	if (!node)
		return;
	
	freeAST(node->next);
	freeAST(node->left);
	freeAST(node->right);
	freeAST(node);
}

void printAST(ASTNode* node) {

}