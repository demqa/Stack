#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>




struct stack_t{
    int *data;
    size_t size;
    size_t capacity;
    int error;
};

const int ADDITIONAL_SIZE = 1;

const int POISON = 0xFAFA;

stack_t *StackCtor(stack_t *stack, size_t capacity);
int StackDtor(stack_t *stack);
int StackPop(stack_t *stack);
int StackPush(stack_t *stack, int elem);
const char *ErrorCodePhrase(int error_code);
int CheckError(stack_t *stack);

int StackVerify(stack_t *stack);

#define case_of_switch(error_code) \
        case error_code:           \
            return #error_code;    

#define STACK_ERROR(error_code) \
        stack->error = error_code; \
        return 0;


// #define DUMP(slave){    \
//     FILE *stream = fopen("LOG.OUT", "a");                        \
//     fprintf(stream, "");              \                                                           
// }



enum ErrorCode{
    STACK_IS_NULLPTR = 0x1000,
    STACK_WITH_ZERO_ELEMS,
    STACK_IS_ALREADY_EMPTY,
    CANT_ALLOCATE_MEMORY,
    STACK_DATA_IS_NULLPTR,
    STACK_DATA_IS_RUINED,
    STACK_IS_DESTRUCTED,
};
