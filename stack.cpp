#include "stack.h"

///////////////////////////////////////////////////////////////////
////////////////////         SOMEHOW          /////////////////////
////// HAVE TO CHECK REPEAT CALL NOT TO ALLOC MEMORY AGAIN ////////
///////////////////////////////////////////////////////////////////

int ConstructStack(stack_t *stack, size_t max_size){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }
    if (max_size == 0){
        stack = nullptr;
        return STACK_WITH_ZERO_ELEMS;
    }

    stack->top = (int *) calloc(max_size, sizeof(int));
    if (stack->top == nullptr){
        return CANT_ALLOCATE_MEMORY;
    }

    stack->size = 0;
    stack->max_size = max_size;

    return 0;
}

int Pop(stack_t *stack){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }
    if (stack->size == 0){
        return STACK_IS_ALREADY_EMPTY;
    }
    
    *stack->top = 0;
    if (stack->size != 1){
        --stack->top;
    }

    --stack->size;

    return 0;
}

int Push(stack_t *stack, int elem){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }
    if (stack->size + 1 > stack->max_size){
        return STACK_IS_OVERFLOWED;
    }

    if (stack->size == 0){
        *stack->top = elem;
    }
    else{
        ++stack->top;
        *stack->top = elem;
    }
    
    
    ++stack->size;

    return 0;
}

int DestructStack(stack_t *stack){

    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }

    while (stack->size > 1){
        *stack->top = stack->size * 0x707A;
        --stack->size;
        --stack->top;
    }
    *stack->top = 0x70CA;

    free(stack->top);
    stack->top = nullptr;

    stack->max_size = 0x1111;
    stack->size     = 0x2222;

    return 0;
}

const char *ErrorCodePhrase(int error_code){
    switch (error_code){
        case 0: return "OK";

        case_of_switch(STACK_IS_NULLPTR)
        case_of_switch(STACK_WITH_ZERO_ELEMS)
        case_of_switch(STACK_IS_ALREADY_EMPTY)
        case_of_switch(STACK_IS_OVERFLOWED)
        
        case_of_switch(CANT_ALLOCATE_MEMORY)

        default: return "UNDEFINED_ERROR";
    }
}


void CheckError(int *error_code){
    if (*error_code){
        const char *phrase = ErrorCodePhrase(*error_code);
        assert(phrase != nullptr);

        printf("error №%x, %s\n", *error_code, phrase);

        *error_code = 0;
    }
}
