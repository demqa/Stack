#include "stack.h"

int main(){
    stack_t stack;
    int error_code = 0;

    size_t size = 4;

    error_code = ConstructStack(&stack, size);

    error_code = Push(&stack, 15);
    CheckError(&error_code);

    error_code = Push(&stack, 1111);
    CheckError(&error_code);

    error_code = Push(&stack, 223);
    CheckError(&error_code);

    error_code = Push(&stack, 330);
    CheckError(&error_code);

    error_code = Push(&stack, 230);
    CheckError(&error_code);

    error_code = Push(&stack, 707);
    CheckError(&error_code);

    error_code = Pop(&stack);
    CheckError(&error_code);

    error_code = Pop(&stack);
    CheckError(&error_code);

    error_code = Pop(&stack);
    CheckError(&error_code);

    error_code = Pop(&stack);
    CheckError(&error_code);

    error_code = Pop(&stack);
    CheckError(&error_code);

    error_code = DestructStack(&stack);
    CheckError(&error_code);


    // DONT DESTRUCT STACK AGAIN PLEASE
    // error_code = DestructStack(&stack);
    // CheckError(&error_code);


    return 0;
}