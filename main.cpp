#include "stack.h"

int main(){

    stack_t stack = {};

    size_t initial_capacity = 5;

    // StackDump(&stack);

    StackCtor(&stack, initial_capacity);

    PRINT_PTR(&stack);
    
    // PRINT_PTR(stack.data);

    // stack.data = (Elem_t *) (0xBADBAD);

    // PRINT_PTR(stack.data);

    // StackDump(&stack);


    // PRINT_LINE;
    StackCtor(&stack, 3);
    // PRINT_LINE;

    StackDump(&stack);
    // PRINT_LINE;

    StackPush(&stack, {15});

    StackPush(&stack, {1337});
    StackPush(&stack, {1337});

    StackPush(&stack, {223});
    StackPush(&stack, {330});
    StackPush(&stack, {230});
    StackPush(&stack, {707});

    // StackDump(&stack);

    StackPush(&stack, {223});
    // StackDump(&stack);
    StackPush(&stack, {330});
    // StackDump(&stack);
    StackPush(&stack, {230});
    StackPush(&stack, {707});
    StackPush(&stack, {223});
    StackPush(&stack, {330});
    StackPush(&stack, {230});
    StackPush(&stack, {707});
    StackPush(&stack, {223});
    // StackDump(&stack);
    StackPush(&stack, {330});
    // StackDump(&stack);
    StackPush(&stack, {230});
    StackPush(&stack, {707});
    

    // StackDump(&stack);

    for (int i = 0; i < 30; i++) StackPush(&stack, {1000 - 7});

    // StackDump(&stack);

    for (int i = 0; i < 20; ++i) StackPop(&stack);

    // PRINT_LINE;

    for (int i = 0; i < 16; ++i) StackPop(&stack);

    // PRINT_LINE;

    StackDump(&stack);

    // PRINT_LINE;

    StackDtor(&stack);

    // PRINT_LINE;

    StackDtor(&stack);
    
    return 0;
}