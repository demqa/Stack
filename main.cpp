#include "stack.h"

int main(){

    stack_t stack = {};

    size_t initial_capacity = 5;

    // StackDump(&stack);

    // StackPush(&stack, 11);
    hack1;

    // StackDump(&stack);

    StackCtor(&stack, 3, nullptr);
    StackCtor(&stack, 3, nullptr);
    StackCtor(&stack, 3, nullptr);
    StackCtor(&stack, 3, nullptr);

    // StackDump(&stack);

    StackPush(&stack, 1);

    // StackDump(&stack);

    // stack.hash_data = 0x32321UL;

    StackPush(&stack, 2);

    // StackDump(&stack);
    
    StackPush(&stack, 3);

    StackPush(&stack, 4);
    StackPush(&stack, 5);
    StackPush(&stack, 6);
    StackPush(&stack, 7);

    StackDump(&stack);

    StackPush(&stack, 8);

    StackDump(&stack);

    StackPush(&stack, 9);

    // StackDump(&stack);

    // PRINT_LINE;

    // StackPush(&stack, {230});

    // StackDump(&stack);

    // for (int i = 0; i < 60; i++) StackPush(&stack, {1000 - 7});

    StackDump(&stack);

    for (int i = 0; i < 40; ++i) StackPop(&stack);

    StackPush(&stack, 123);

    StackDump(&stack);

    // PRINT_LINE;

    // for (int i = 0; i < 16; ++i) StackPop(&stack);

    // PRINT_LINE;

    StackDump(&stack);

    // PRINT_LINE;

    // StackDtor(&stack);

    // PRINT_LINE;

    StackDtor(&stack);

    StackDtor(&stack);

    StackDtor(&stack);

    // PRINT_LINE;

    StackDump(&stack);
    
    return 0;
}