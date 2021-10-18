#include "stack.h"
#include "print_func.cpp"


int main(){
    stack_t stack = {};
    
    StackCtor(&stack, 1, nullptr);

    for (int i = 0; i < 2; i++){
        StackPush(&stack, 0xDED32DED);
        StackDump(&stack);
    }

    for (int i = 0; i < 5; i++){
        StackPop(&stack);
    }

    StackDump(&stack);

    StackDtor(&stack);

    return 0;
}
