#include "stack.h"

int main(){

    stack_t stack = {};

    size_t initial_capacity = 0;

    StackDump(&stack);

    StackCtor(&stack, initial_capacity);

    StackDump(&stack);

    PRINT_LINE;
    StackCtor(&stack, 123);
    PRINT_LINE;

    StackDump(&stack);
    PRINT_LINE;

    // StackPush(&stack, {15});

    // StackPush(&stack, {1337});
    // StackPush(&stack, {1337});

    // StackPush(&stack, {223});
    // StackPush(&stack, {330});
    // StackPush(&stack, {230});
    // StackPush(&stack, {707});

    // StackDump(&stack);

    // for (int i = 0; i < 30; i++) StackPush(&stack, {1000 - 7});


    // for (int i = 0; i < 20; ++i) StackPop(&stack);



    // StackDtor(&stack);

    StackDtor(&stack);
    
    return 0;
}