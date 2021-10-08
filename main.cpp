#include "stack.h"

int main(){

    stack_t stack = {};

    size_t initial_capacity = 0;
    
    
    // StackDump(&stack);
    // PRINT_LINE;

    StackCtor(&stack, initial_capacity);
    // PRINT_LINE;
      
    // StackDump(&stack);
    // PRINT_LINE;
    // check how not to initialize stack again

    StackPush(&stack, {15});
    // PRINT_LINE;

    // PRINT_LINE;

    // StackDump(&stack);

    StackPush(&stack, {1488});
    StackPush(&stack, {1488});
    StackPush(&stack, {1488});
    StackPush(&stack, {1488});
    StackPush(&stack, {1488});

    StackPush(&stack, {223});

    StackPush(&stack, {330});

    StackPush(&stack, {230});

    StackPush(&stack, {707});

    
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});
    StackPush(&stack, {1000 - 7});


    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);
    StackPop(&stack);


    StackDtor(&stack);

    StackDtor(&stack);
    
    return 0;
}