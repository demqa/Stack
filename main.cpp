#include "stack.h"

int main(){

    stack_t stack = {};

    size_t initial_capacity = 0;
    
    
    StackDump(&stack);

    StackCtor(&stack, initial_capacity);
      
    StackDump(&stack);
    // check how not to initialize stack again

    StackPush(&stack, {15});
       // think how not to check error every time...
                        // I have verificator now
    // PRINT_LINE;

    StackDump(&stack);

    StackPush(&stack, {1488});
    StackPush(&stack, {1488});
    StackPush(&stack, {1488});
    StackPush(&stack, {1488});
    StackPush(&stack, {1488});

    // PRINT_LINE;

    StackDump(&stack);

    StackPush(&stack, {223});
      
    // PRINT_LINE;

    StackDump(&stack);


    StackPush(&stack, {330});  
    // PRINT_LINE;

    StackPush(&stack, {230}); 
    // PRINT_LINE;

    StackPush(&stack, {707}); 
    // PRINT_LINE;

    printf("popped num is %d\n", StackPop(&stack).num);
    // PRINT_LINE;
    StackDump(&stack);

    printf("popped num is %d\n", StackPop(&stack).num);    
    // PRINT_LINE;
    StackDump(&stack);

    printf("popped num is %d\n", StackPop(&stack).num);    
    // PRINT_LINE;
    StackDump(&stack);

    printf("popped num is %d\n", StackPop(&stack).num);     
    // PRINT_LINE;
    StackDump(&stack);

    printf("popped num is %d\n", StackPop(&stack).num);     
    // PRINT_LINE;
    StackDump(&stack);

    printf("popped num is %d\n", StackPop(&stack).num);   
    // PRINT_LINE;
    StackDump(&stack);

    printf("popped num is %d\n", StackPop(&stack).num);   
    // PRINT_LINE;
    StackDump(&stack);

    // PRINT_LINE;
    StackDtor(&stack);
    // PRINT_LINE;

    StackDump(&stack);
    // PRINT_LINE;
    StackDtor(&stack);
      
    // PRINT_LINE;

    // DONT DESTRUCT STACK AGAIN // PLEASE
    // DestructStack(&stack);
    // CheckError(&stack);;
    

    return 0;
}