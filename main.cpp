#include "stack.h"

int main(){

    stack_t stack;

    size_t size = 4;

    ConstructStack(&stack, size);
    CheckError(&stack);

    Push(&stack, 15);
    CheckError(&stack);

    Push(&stack, 1111);
    CheckError(&stack);

    Push(&stack, 223);
    CheckError(&stack);

    Push(&stack, 330);
    CheckError(&stack);

    Push(&stack, 230);
    CheckError(&stack);

    Push(&stack, 707);
    CheckError(&stack);

    Pop(&stack);
    CheckError(&stack);

    Pop(&stack);
    CheckError(&stack);

    Pop(&stack);
    CheckError(&stack);

    Pop(&stack);
    CheckError(&stack);

    Pop(&stack);
    CheckError(&stack);

    DestructStack(&stack);
    CheckError(&stack);

    DestructStack(&stack);
    CheckError(&stack);

    // DONT DESTRUCT STACK AGAIN PLEASE
    // DestructStack(&stack);
    // CheckError(&stack);;
    

    return 0;
}