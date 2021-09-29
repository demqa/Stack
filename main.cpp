#include "stack.h"

int main(){

    stack_t stack = {};

    size_t initial_capacity = 1;
    
    
    // StackDump(&stack);

    StackCtor(&stack, initial_capacity);
    CheckError(&stack);

    StackDump(&stack);
    // check how not to initialize stack again

    StackPush(&stack, {15});
    CheckError(&stack); // think how not to check error every time...
                        // I have verificator now
    printf("I am at line #%d\n\n", __LINE__);


    StackPush(&stack, {1111});
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    StackPush(&stack, {223});
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    StackPush(&stack, {330});
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    StackPush(&stack, {230});
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    StackPush(&stack, {707});
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", StackPop(&stack).num);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    StackDtor(&stack);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    StackDtor(&stack);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    // DONT DESTRUCT STACK AGAIN PLEASE
    // DestructStack(&stack);
    // CheckError(&stack);;
    

    return 0;
}