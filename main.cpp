#include "stack.h"
#include "print_func.cpp"

int main(){
    stack_t stack = {};
    
    StackCtor(&stack, 12, nullptr);

    StackCtor(&stack, 12, nullptr);

    StackCtor(&stack, 12, nullptr);

    StackPop(&stack);

    // StackDump(&stack);

    StackPush(&stack, 123);

    // StackDump(&stack);

    StackPop(&stack);

    //StackDump(&stack);

    StackPop(&stack);

    StackPop(&stack);

    for (int i = 0; i < 10; ++i){
        StackPush(&stack, 1337);
        // StackDump(&stack);
    }

    for (int i = 0; i < 20; ++i) StackPush(&stack, 1488);

    StackPush(&stack, 23); StackPush(&stack, 23); StackPush(&stack, 23);

    for (int i = 0; i < 35; i++) StackPop(&stack);

    StackDtor(&stack);

    return 0;
}
