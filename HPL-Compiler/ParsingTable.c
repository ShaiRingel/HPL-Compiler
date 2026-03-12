#include "ParsingTable.h"
#include <stdlib.h>
#include <stdio.h>

static void freeSymbolList(ActionNode* head);
static void freeStateRow(StateRow* row);


static StateRow* allocateStateRow(void) {
    StateRow* row;
    row = (StateRow*)malloc(sizeof(StateRow));
    if (!row) exit(EXIT_FAILURE);

    row->capacity = CAPACITY_SYMBOLS;
    row->symbolBuckets = (ActionNode**)calloc(CAPACITY_SYMBOLS, sizeof(ActionNode*));
    if (!row->symbolBuckets) exit(EXIT_FAILURE);

    return row;
}

static StateNode* allocateStateNode(unsigned short stateId) {
    StateNode* node;
    node = (StateNode*)malloc(sizeof(StateNode));
    if (!node) exit(EXIT_FAILURE);

    node->stateId = stateId;
    node->row = allocateStateRow();
    node->next = NULL;

    return node;
}

static void freeSymbolList(ActionNode* head) {
    ActionNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

static void freeStateRow(StateRow* row) {
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
    unsigned int idx;
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
    unsigned int idx;
    ActionNode* curr;

    idx = hashNumber(symbol, row->capacity);
    curr = row->symbolBuckets[idx];

    while (curr) {
        if (curr->symbol == symbol) return curr;
        curr = curr->next;
    }
    return NULL;
}

static void addNewTransition(StateRow* row, int symbol, ActionType type, int value) {
    unsigned int idx;
    ActionNode* newNode;

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
    StateNode* stateNode;
    ActionNode* ActionNode;

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
    unsigned int stateIdx, symbolIdx;
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

    case 0:  return 1; // PROG -> STMT_LIST

    case 1:  return 3; // BLOCK -> INDENT STMT_LIST DEDENT

    case 2:  return 1; // STMT_LIST -> STMT
    case 3:  return 2; // STMT_LIST -> STMT_LIST STMT

    case 4:  return 1; // STMT -> VAR_DECL
    case 5:  return 1; // STMT -> ASSIGN_STMT
    case 6:  return 1; // STMT -> IO_STMT
    case 7:  return 1; // STMT -> CTRL_FLOW
    case 8:  return 1; // STMT -> FUNC_DECL
    case 9:  return 1; // STMT -> FUNC_RET
    case 10: return 1; // STMT -> FUNC_CALL_STMT

    case 11: return 5; // VAR_DECL -> Let id be TYPE .

    case 12: return 1; // TYPE -> number
    case 13: return 1; // TYPE -> text

    case 14: return 5; // ASSIGN_STMT -> Set L_VAL to E .
    case 15: return 5; // ASSIGN_STMT -> Increase L_VAL by E .
    case 16: return 5; // ASSIGN_STMT -> Decrease L_VAL by E .
    case 17: return 5; // ASSIGN_STMT -> Multiply L_VAL by E .
    case 18: return 5; // ASSIGN_STMT -> Divide L_VAL by E .

    case 19: return 1; // L_VAL -> id
    case 20: return 5; // L_VAL -> id atPosition ( E )

    case 21: return 3; // IO_STMT -> Show E .
    case 22: return 7; // IO_STMT -> Get TYPE ask TEXT_LIT storeInto L_VAL .

    case 23: return 1; // CTRL_FLOW -> IF_STMT
    case 24: return 1; // CTRL_FLOW -> LOOP_STMT

    case 25: return 6; // IF_STMT -> If COND then : BLOCK ELIF_LIST

    case 26: return 7; // ELIF_LIST -> Otherwise if COND then : BLOCK ELIF_LIST
    case 27: return 1; // ELIF_LIST -> ELSE_PART

    case 28: return 3; // ELSE_PART -> Otherwise : BLOCK
    case 29: return 0; // ELSE_PART -> ε

    case 30: return 5; // LOOP_STMT -> Repeat E iterations : BLOCK
    case 31: return 5; // LOOP_STMT -> Foreach id in id : BLOCK
    case 32: return 4; // LOOP_STMT -> While COND : BLOCK

    case 33: return 6; // FUNC_DECL -> To id with PARAM_LIST : BLOCK
    case 34: return 6; // FUNC_DECL -> To id with nan : BLOCK

    case 35: return 1; // PARAM_LIST -> id
    case 36: return 3; // PARAM_LIST -> PARAM_LIST , id

    case 37: return 3; // FUNC_RET -> Return E .

    case 38: return 2; // FUNC_CALL_STMT -> FUNC_CALL .

    case 39: return 5; // FUNC_CALL -> id with ( ARG_LIST )
    case 40: return 5; // FUNC_CALL -> id with ( nan )

    case 41: return 1; // ARG_LIST -> E
    case 42: return 3; // ARG_LIST -> ARG_LIST , E

    case 43: return 3; // E -> E E_OP T
    case 44: return 1; // E -> T

    case 45: return 3; // T -> T T_OP F
    case 46: return 4; // T -> remainderOf F dividedBy F
    case 47: return 1; // T -> F

    case 48: return 1; // F -> L_VAL
    case 49: return 1; // F -> FUNC_CALL
    case 50: return 1; // F -> NUMBER_LIT
    case 51: return 1; // F -> TEXT_LIT
    case 52: return 3; // F -> ( E )

    case 53: return 3; // COND -> COND or BOOL_T
    case 54: return 1; // COND -> BOOL_T

    case 55: return 3; // BOOL_T -> BOOL_T and BOOL_F
    case 56: return 1; // BOOL_T -> BOOL_F

    case 57: return 3; // BOOL_F -> E REL_OP E
    case 58: return 2; // BOOL_F -> not BOOL_F
    case 59: return 3; // BOOL_F -> ( COND )

    case 60: return 1; // E_OP -> plus
    case 61: return 1; // E_OP -> minus

    case 62: return 1; // T_OP -> times
    case 63: return 1; // T_OP -> divide

    case 64: // REL_OP -> equalsTo
    case 65: // REL_OP -> notEqualsTo
    case 66: // REL_OP -> greaterThan
    case 67: // REL_OP -> lessThan
    case 68: // REL_OP -> atLeast
    case 69: // REL_OP -> atMost
        return 1;

    default:
        return 0;
    }
}

int getLHS(int ruleId) {

    if (ruleId == 0) return NON_TERMINAL_PROG;

    if (ruleId == 1) return NON_TERMINAL_BLOCK;

    if (ruleId >= 2 && ruleId <= 3)
        return NON_TERMINAL_STMT_LIST;

    if (ruleId >= 4 && ruleId <= 10)
        return NON_TERMINAL_STMT;

    if (ruleId == 11)
        return NON_TERMINAL_VAR_DECL;

    if (ruleId >= 12 && ruleId <= 13)
        return NON_TERMINAL_TYPE;

    if (ruleId >= 14 && ruleId <= 18)
        return NON_TERMINAL_ASSIGN_STMT;

    if (ruleId >= 19 && ruleId <= 20)
        return NON_TERMINAL_LVAL;

    if (ruleId >= 21 && ruleId <= 22)
        return NON_TERMINAL_IO_STMT;

    if (ruleId >= 23 && ruleId <= 24)
        return NON_TERMINAL_CTRL_FLOW;

    if (ruleId == 25)
        return NON_TERMINAL_IF_STMT;

    if (ruleId >= 26 && ruleId <= 27)
        return NON_TERMINAL_ELIF_LIST;

    if (ruleId >= 28 && ruleId <= 29)
        return NON_TERMINAL_ELSE_PART;

    if (ruleId >= 30 && ruleId <= 32)
        return NON_TERMINAL_LOOP_STMT;

    if (ruleId >= 33 && ruleId <= 34)
        return NON_TERMINAL_FUNC_DECL;

    if (ruleId >= 35 && ruleId <= 36)
        return NON_TERMINAL_PARAM_LIST;

    if (ruleId == 37)
        return NON_TERMINAL_FUNC_RET;

    if (ruleId == 38)
        return NON_TERMINAL_FUNC_CALL_STMT;

    if (ruleId >= 39 && ruleId <= 40)
        return NON_TERMINAL_FUNC_CALL;

    if (ruleId >= 41 && ruleId <= 42)
        return NON_TERMINAL_ARG_LIST;

    if (ruleId >= 43 && ruleId <= 44)
        return NON_TERMINAL_E;

    if (ruleId >= 45 && ruleId <= 47)
        return NON_TERMINAL_T;

    if (ruleId >= 48 && ruleId <= 52)
        return NON_TERMINAL_F;

    if (ruleId >= 53 && ruleId <= 54)
        return NON_TERMINAL_COND;

    if (ruleId >= 55 && ruleId <= 56)
        return NON_TERMINAL_BOOL_T;

    if (ruleId >= 57 && ruleId <= 59)
        return NON_TERMINAL_BOOL_F;

    if (ruleId >= 60 && ruleId <= 61)
        return NON_TERMINAL_E_OP;

    if (ruleId >= 62 && ruleId <= 63)
        return NON_TERMINAL_T_OP;

    if (ruleId >= 64 && ruleId <= 69)
        return NON_TERMINAL_REL_OP;

    return -1;
}