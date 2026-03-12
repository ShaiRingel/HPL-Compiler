#include "ParsingDefinitions.h"
#include "HashingFunctions.h"
#include "ParsingTable.h"
#include <stdlib.h>
#include <stdio.h>

void freeSymbolList(ActionNode* head);
void freeStateRow(StateRow* row);


StateRow* allocateStateRow(void) {
    StateRow* row;
    row = (StateRow*)malloc(sizeof(StateRow));
    if (!row) exit(EXIT_FAILURE);

    row->capacity = CAPACITY_SYMBOLS;
    row->symbolBuckets = (ActionNode**)calloc(CAPACITY_SYMBOLS, sizeof(ActionNode*));
    if (!row->symbolBuckets) exit(EXIT_FAILURE);

    return row;
}

StateNode* allocateStateNode(unsigned short stateId) {
    StateNode* node;
    node = (StateNode*)malloc(sizeof(StateNode));
    if (!node) exit(EXIT_FAILURE);

    node->stateId = stateId;
    node->row = allocateStateRow();
    node->next = NULL;

    return node;
}

void freeSymbolList(ActionNode* head) {
    ActionNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeStateRow(StateRow* row) {
    int i;
    if (!row) return;
    for (i = 0; i < row->capacity; i++) {
        freeSymbolList(row->symbolBuckets[i]);
    }
    free(row->symbolBuckets);
    free(row);
}

void freeParsingTable(ParsingTable* table) {
    int i;
    StateNode* curr, *temp;
    if (!table) return;

    for (i = 0; i < table->capacity; i++) {
        curr = table->stateBuckets[i];
        while (curr) {
            temp = curr;
            curr = curr->next;
            freeStateRow(temp->row);
            free(temp);
        }
    }
    free(table->stateBuckets);
    free(table);
}

ParsingTable* initParsingTable(void) {
    ParsingTable* table;
    table = (ParsingTable*)malloc(sizeof(ParsingTable));

    if (!table) {
        fprintf(stderr, "Error: Table allocation failed\n");
        exit(EXIT_FAILURE);
    }

    table->capacity = CAPACITY_STATES;
    table->stateBuckets = (StateNode**)calloc(CAPACITY_STATES, sizeof(StateNode*));

    if (!table->stateBuckets) {
        free(table);
        exit(EXIT_FAILURE);
    }

    return table;
}

static StateNode* findOrCreateState(ParsingTable* table, unsigned short stateId) {
    unsigned idx;
    StateNode* curr;

    idx = hashNumber(stateId, table->capacity);
    curr = table->stateBuckets[idx];

    while (curr) {
        if (curr->stateId == stateId) return curr;
        curr = curr->next;
    }

    /* Not found: Create and prepend to the linked list */
    curr = allocateStateNode(stateId);
    curr->next = table->stateBuckets[idx];
    table->stateBuckets[idx] = curr;

    return curr;
}

static ActionNode* findSymbolInRow(StateRow* row, int symbol) {
    ActionNode* curr;
    unsigned idx;

    idx = hashNumber(symbol, row->capacity);
    curr = row->symbolBuckets[idx];

    while (curr) {
        if (curr->symbol == symbol) return curr;
        curr = curr->next;
    }
    return NULL;
}

static void addNewTransition(StateRow* row, int symbol, ActionType type, int value) {
    ActionNode* newNode;
    unsigned idx;

    newNode = (ActionNode*)malloc(sizeof(ActionNode));
    if (!newNode) exit(EXIT_FAILURE);

    newNode->symbol = symbol;
    newNode->action.type = type;
    newNode->action.value = value;

    idx = hashNumber(symbol, row->capacity);
    newNode->next = row->symbolBuckets[idx];
    row->symbolBuckets[idx] = newNode;
}

void insertTableEntry(ParsingTable* table, unsigned short stateId, int symbol, ActionType type, int value) {
    ActionNode* ActionNode;
    StateNode* stateNode;

    if (!table) return;

    stateNode = findOrCreateState(table, stateId);

    ActionNode = findSymbolInRow(stateNode->row, symbol);

    if (ActionNode) {
        ActionNode->action.type = type;
        ActionNode->action.value = value;
    }
    else {
        addNewTransition(stateNode->row, symbol, type, value);
    }
}

ParseAction getEntry(ParsingTable* table, unsigned short stateId, int symbol) {
    unsigned stateIdx, symbolIdx;
    StateNode* stateNode;
    ActionNode* symNode;
    ParseAction errAction;

    errAction.type = ACTION_ERROR;
    errAction.value = 0;

    if (!table) return errAction;

    stateIdx = hashNumber(stateId, table->capacity);
    stateNode = table->stateBuckets[stateIdx];

    while (stateNode) {
        if (stateNode->stateId == stateId) {
            symbolIdx = hashNumber(symbol, stateNode->row->capacity);
            symNode = stateNode->row->symbolBuckets[symbolIdx];

            while (symNode) {
                if (symNode->symbol == symbol) return symNode->action;
                symNode = symNode->next;
            }
            return errAction;
        }
        stateNode = stateNode->next;
    }

    return errAction;
}

int getRuleLength(int ruleId) {
    switch (ruleId) {

    case 0:  return 1;  // PROG -> STMT_LIST

    case 1:  return 3;  // BLOCK -> INDENT STMT_LIST DEDENT

    case 2:  return 1;  // STMT_LIST -> STMT
    case 3:  return 2;  // STMT_LIST -> STMT_LIST STMT

    case 4:             // STMT -> VAR_DECL
    case 5:             // STMT -> ASSIGN_STMT
    case 6:             // STMT -> IO_STMT
    case 7:             // STMT -> CTRL_FLOW
    case 8:             // STMT -> FUNC_DECL
    case 9:             // STMT -> FUNC_RET
    case 10: return 1;  // STMT -> FUNC_CALL_STMT

    case 11: return 5;  // VAR_DECL -> Let id be TYPE .

    case 12:            // TYPE -> number
    case 13: return 1;  // TYPE -> text

    case 14:            // ASSIGN_STMT -> Set L_VAL to E .
    case 15:            // ASSIGN_STMT -> Increase L_VAL by E .
    case 16:            // ASSIGN_STMT -> Decrease L_VAL by E .
    case 17:            // ASSIGN_STMT -> Multiply L_VAL by E .
    case 18: return 5;  // ASSIGN_STMT -> Divide L_VAL by E .

    case 19: return 1;  // L_VAL -> id
    case 20: return 5;  // L_VAL -> id atPosition ( E )

    case 21: return 3;  // IO_STMT -> Show E .
    case 22: return 7;  // IO_STMT -> Get TYPE ask TEXT_LIT storeInto L_VAL .

    case 23: return 1;  // CTRL_FLOW -> IF_STMT
    case 24: return 1;  // CTRL_FLOW -> LOOP_STMT

    case 25: return 6;  // IF_STMT -> If COND then : BLOCK ELIF_LIST

    case 26: return 7;  // ELIF_LIST -> Otherwise if COND then : BLOCK ELIF_LIST
    case 27: return 1;  // ELIF_LIST -> ELSE_PART

    case 28: return 3;  // ELSE_PART -> Otherwise : BLOCK
    case 29: return 0;  // ELSE_PART -> ε

    case 30: return 5;  // LOOP_STMT -> Repeat E iterations : BLOCK
    case 31: return 6;  // LOOP_STMT -> Foreach id in id : BLOCK
    case 32: return 4;  // LOOP_STMT -> While COND : BLOCK

    case 33:            // FUNC_DECL -> To id with PARAM_LIST : BLOCK
    case 34: return 6;  // FUNC_DECL -> To id with nan : BLOCK

    case 35: return 1;  // PARAM_LIST -> id
    case 36: return 3;  // PARAM_LIST -> PARAM_LIST , id

    case 37: return 3;  // FUNC_RET -> Return E .

    case 38: return 2;  // FUNC_CALL_STMT -> FUNC_CALL .

    case 39:            // FUNC_CALL -> id with ( ARG_LIST )
    case 40: return 5;  // FUNC_CALL -> id with ( nan )

    case 41: return 1;  // ARG_LIST -> E
    case 42: return 3;  // ARG_LIST -> ARG_LIST , E

    case 43: return 3;  // E -> E E_OP T
    case 44: return 1;  // E -> T

    case 45: return 3;  // T -> T T_OP F
    case 46: return 4;  // T -> remainderOf F dividedBy F
    case 47: return 1;  // T -> F

    case 48:            // F -> L_VAL
    case 49:            // F -> FUNC_CALL
    case 50:            // F -> NUMBER_LIT
    case 51: return 1;  // F -> TEXT_LIT
    case 52: return 3;  // F -> ( E )

    case 53: return 3;  // COND -> COND or BOOL_T
    case 54: return 1;  // COND -> BOOL_T

    case 55: return 3;  // BOOL_T -> BOOL_T and BOOL_F
    case 56: return 1;  // BOOL_T -> BOOL_F

    case 57: return 3;  // BOOL_F -> E REL_OP E
    case 58: return 2;  // BOOL_F -> not BOOL_F
    case 59: return 3;  // BOOL_F -> ( COND )

    case 60:            // E_OP -> plus
    case 61: return 1;  // E_OP -> minus

    case 62:            // T_OP -> times
    case 63: return 1;  // T_OP -> divide

    case 64:            // REL_OP -> equalsTo
    case 65:            // REL_OP -> notEqualsTo
    case 66:            // REL_OP -> greaterThan
    case 67:            // REL_OP -> lessThan
    case 68:            // REL_OP -> atLeast
    case 69:            // REL_OP -> atMost
        return 1;

    default:
        return 0;
    }
}

int getLHS(int ruleId) {
    switch (ruleId) {

    case 0:
        return NON_TERMINAL_PROG;

    case 1:
        return NON_TERMINAL_BLOCK;

    case 2: case 3:
        return NON_TERMINAL_STMT_LIST;

    case 4: case 5: case 6: case 7: case 8: case 9: case 10:
        return NON_TERMINAL_STMT;

    case 11:
        return NON_TERMINAL_VAR_DECL;

    case 12: case 13:
        return NON_TERMINAL_TYPE;

    case 14: case 15: case 16: case 17: case 18:
        return NON_TERMINAL_ASSIGN_STMT;

    case 19: case 20:
        return NON_TERMINAL_LVAL;

    case 21: case 22:
        return NON_TERMINAL_IO_STMT;

    case 23: case 24:
        return NON_TERMINAL_CTRL_FLOW;

    case 25:
        return NON_TERMINAL_IF_STMT;

    case 26: case 27:
        return NON_TERMINAL_ELIF_LIST;

    case 28: case 29:
        return NON_TERMINAL_ELSE_PART;

    case 30: case 31: case 32:
        return NON_TERMINAL_LOOP_STMT;

    case 33: case 34:
        return NON_TERMINAL_FUNC_DECL;

    case 35: case 36:
        return NON_TERMINAL_PARAM_LIST;

    case 37:
        return NON_TERMINAL_FUNC_RET;

    case 38:
        return NON_TERMINAL_FUNC_CALL_STMT;

    case 39: case 40:
        return NON_TERMINAL_FUNC_CALL;

    case 41: case 42:
        return NON_TERMINAL_ARG_LIST;

    case 43: case 44:
        return NON_TERMINAL_E;

    case 45: case 46: case 47:
        return NON_TERMINAL_T;

    case 48: case 49: case 50: case 51: case 52:
        return NON_TERMINAL_F;

    case 53: case 54:
        return NON_TERMINAL_COND;

    case 55: case 56:
        return NON_TERMINAL_BOOL_T;

    case 57: case 58: case 59:
        return NON_TERMINAL_BOOL_F;

    case 60: case 61:
        return NON_TERMINAL_E_OP;

    case 62: case 63:
        return NON_TERMINAL_T_OP;

    case 64: case 65: case 66: case 67: case 68: case 69:
        return NON_TERMINAL_REL_OP;

    default:
        return -1;
    }
}