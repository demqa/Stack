#include "stack.h"

int main(){

    stack_t stack;

    size_t size = 1;

    ConstructStack(&stack, size);
    CheckError(&stack);

    Push(&stack, 15);
    CheckError(&stack); // think how not to check error every time...
                        // I have verificator now
    printf("I am at line #%d\n\n", __LINE__);


    Push(&stack, 1111);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    Push(&stack, 223);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    Push(&stack, 330);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    Push(&stack, 230);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    Push(&stack, 707);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    printf("popped num is %d\n", Pop(&stack));
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    DestructStack(&stack);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    DestructStack(&stack);
    CheckError(&stack);
    printf("I am at line #%d\n\n", __LINE__);

    // DONT DESTRUCT STACK AGAIN PLEASE
    // DestructStack(&stack);
    // CheckError(&stack);;
    

    return 0;
}