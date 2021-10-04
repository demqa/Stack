#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

enum StatusCode{
    STACK_IS_OK         = 0,
    DUMP_COMMITED       = 1 << 31,
    STACK_IS_DESTRUCTED = 1 << 30,
    STACK_IS_EMPTY      = 1 << 29,
    RESULT_IS_UNKNOWN   = -1,

    STACK_IS_NULLPTR                = 1 << 0,
    STACK_IS_ALREADY_EMPTY          = 1 << 1,
    CANT_ALLOCATE_MEMORY            = 1 << 2,
    STACK_DATA_IS_RUINED            = 1 << 3,
    STACK_DATA_IS_NULLPTR           = 1 << 4,
    STACK_SIZE_BIGGER_THAN_CAPACITY = 1 << 5,
    STACK_CAPACITY_LESS_THAN_ZERO   = 1 << 6,
    STACK_SIZE_LESS_THAN_ZERO       = 1 << 7,

    
};

#define DEBUG_MODE 01

// debug mode & 01 
// debug mode & 02
// debug mode & 04
// debug mode & 010

const int STRING_MAX_SIZE = 100;

struct Elem_t{
    int num;
};

// i can do POISONED_ELEM WITH MACRO
const Elem_t POISONED_ELEM = {
    666,
};

#if DEBUG_MODE & 01
    struct StackInfo{
        int line;
        const char *file;
        const char *func;
        const char *name;
    };
#endif

struct stack_t{
    Elem_t *data;
    size_t size;
    size_t capacity;
    int status;

    #if DEBUG_MODE & 01
        StackInfo info;
    #endif
};

#if DEBUG_MODE & 01
    #define ASSERT_OK(stack){                    \
        if (StackVerify(stack) != STACK_IS_OK){  \
            StackDump(stack);                      \
            assert(!"ok" && "Bad stack");           \
        }                                            \
    }
#endif

const int ADDITIONAL_SIZE = 1;

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]);
StatusCode StackDtor(stack_t *stack);
Elem_t StackPop(stack_t *stack);
StatusCode StackPush(stack_t *stack, Elem_t value);

StatusCode StackIsDestructed(stack_t *stack);
StatusCode StackIsEmpty(stack_t *stack);
int StackVerify(stack_t *stack);

const char *StackStatusPhrase(int error_code);
StatusCode CheckError(stack_t *stack);
StatusCode StackDump_(stack_t *stack, int line, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE]);

#define case_of_switch(error_code) \
        case error_code:           \
            return #error_code;    

#define STACK_STATUS(status_code){   \
    stack->status = status_code;      \
    return (StatusCode) stack->status;             \
}

#if DEBUG_MODE & 01
    #define StackCtor(stack, capacity){                                               \
        StackCtor_(stack, capacity, __LINE__, __FILE__, __PRETTY_FUNCTION__, #stack);  \
    }
#else
    #define StackCtor(stack, capacity){                                               \
        StackCtor_(stack, capacity, 0, "", "", "");                                    \
    }
#endif


#define StackDump(stack){                                                        \
    StackDump_(stack, __LINE__, __FILE__, __PRETTY_FUNCTION__);                   \
}

#define PRINT_LINE printf("I'm at %s at line %d\n", __PRETTY_FUNCTION__, __LINE__);

#define PRINT_ERROR(error){   \
    if (stack_status & error)  \
        printf("%s ", #error);  \
}
