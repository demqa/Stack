#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

struct stack_t{
    int *top;
    size_t size;
    size_t max_size;
    
};

int ConstructStack(stack_t *stack, size_t max_size);
int DestructStack(stack_t *stack);
int Pop(stack_t *stack);
int Push(stack_t *stack, int elem);
const char *ErrorCodePhrase(int error_code);
void CheckError(int *error_code);

#define case_of_switch(error_code) \
        case error_code:           \
            return #error_code;    


enum ErrorCode{
    STACK_IS_NULLPTR = 0x1000,
    STACK_WITH_ZERO_ELEMS,
    STACK_IS_ALREADY_EMPTY,
    STACK_IS_OVERFLOWED,
    CANT_ALLOCATE_MEMORY,

};
