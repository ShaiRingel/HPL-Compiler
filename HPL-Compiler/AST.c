#include "AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* createNode(NodeType type) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));

	if (!node) {
		perror("Malloc failed!");
		exit(EXIT_FAILURE);
	}

	memset(node, NULL, sizeof(node));
	node->type = type;
	node->next = NULL;

	return node;
}

ASTNode* createBinOpNode(OpType op, ASTNode* left, ASTNode* right) {
	ASTNode* node = createNode(NODE_BIN_OP);
	node->val.op = op;
	node->left = left;
	node->right = right;

	return node;
}

ASTNode* createLiteralNode(int val) {
	ASTNode* node = createNode(NODE_LITERAL_INT);
	node->val.intValue = val;
	
	return node;
}

ASTNode* createIdNode(char* name) {
	ASTNode* node = createNode(NODE_IDENTIFIER);
	node->val.stringValue = _strdup(name);

	return node;
}

ASTNode* createAssignmentNode(char* varName, ASTNode* expression) {
	ASTNode* node = createNode(NODE_ASSIGN_STMT);
	node->left = createIdNode(varName);
	node->right = expression;

	return node;
}

ASTNode* createVariableDeclarationNode(char* varName) {
	ASTNode* node = createNode(NODE_VAR_DECL);
	node->left = createIdNode(varName);

	return node;
}

ASTNode* createIfNode(ASTNode* condition, ASTNode* body, ASTNode* elseBody) {
	ASTNode* node = createNode(NODE_IF_STMT);
	
	node->condition = condition;
	node->body = body;
	node->elseBody = elseBody;

	return node;
}

void freeAST(ASTNode* root) {
	if (!root)
		return;

	freeAST(root->left);
	freeAST(root->right);
	freeAST(root->condition);
	freeAST(root->body);
	freeAST(root->elseBody);
	freeAST(root->next);
	
	free(root);
}

void printAST(ASTNode* node, int level) {
	if (!node) return;

	for (int i = 0; i < level; i++) printf("  ");

	switch (node->type) {
		case NODE_BIN_OP:
			printf("OP: %d\n", node->val.op);
			printAST(node->left, level + 1);
			printAST(node->right, level + 1);
			break;
		case NODE_LITERAL_INT:
			printf("INT: %d\n", node->val.intValue);
			break;
		case NODE_IDENTIFIER:
			printf("ID: %s\n", node->val.stringValue);
			break;
		case NODE_ASSIGN_STMT:
			printf("ASSIGN to %s\n", node->left->val.stringValue);
			printAST(node->right, level + 1);
			break;
		case NODE_IF_STMT:
			printf("IF\n");

			for (int i = 0; i < level + 1; i++) printf("  ");
			printf("COND:\n");
			printAST(node->condition, level + 2);

			for (int i = 0; i < level + 1; i++) printf("  ");
			printf("THEN:\n");
			printAST(node->body, level + 2);

			if (node->elseBody) {
				for (int i = 0; i < level + 1; i++) printf("  ");
				printf("ELSE:\n");
				printAST(node->elseBody, level + 2);
			}
			break;

		default:
			printf("Node Type: %d\n", node->type);
	}

	if (node->next)
		printAST(node->next, level);
}