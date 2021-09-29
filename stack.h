#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

enum ErrorCode{
    STACK_IS_OK = 0,
    DUMP_COMMITED = 0,
    STACK_IS_EMPTY = 0x11,
    STACK_IS_NULLPTR = 0x1000,
    STACK_WITH_ZERO_ELEMS,
    STACK_IS_ALREADY_EMPTY,
    CANT_ALLOCATE_MEMORY,
    STACK_DATA_IS_NULLPTR,
    STACK_DATA_IS_RUINED,
    STACK_IS_DESTRUCTED,
    POISON = 0xFAFA,
};

#define DEBUG_MODE 1

const int STRING_MAX_SIZE = 100;

struct StackInfo{
    int line;
    const char *file;
    const char *func;
    const char *name;
};

struct Elem_t{
    int num;
};

const Elem_t POISONED_ELEM = {
    666,
};


struct stack_t{
    Elem_t *data;
    size_t size;
    size_t capacity;
    ErrorCode error;

    #ifdef DEBUG_MODE
    StackInfo info;
    #endif
};

struct debug{

};

#define ASSERT_OK(stack){             \
    if (StackVerify(stack)){           \
        StackDump(stack);               \
        assert(!"ok" && "Bad stack");    \
    }                                     \
}

const int ADDITIONAL_SIZE = 1;

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]);
ErrorCode StackDtor(stack_t *stack);
Elem_t StackPop(stack_t *stack);
ErrorCode StackPush(stack_t *stack, Elem_t value);

ErrorCode StackIsDestructed(stack_t *stack);
ErrorCode StackVerify(stack_t *stack);

const char *ErrorCodePhrase(int error_code);
ErrorCode CheckError(stack_t *stack);
ErrorCode StackDump_(stack_t *stack, int line, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE]);

#define case_of_switch(error_code) \
        case error_code:           \
            return #error_code;    

#define STACK_ERROR(error_code){   \
    stack->error = error_code;      \
    return stack->error;             \
}
        


#ifdef DEBUG_MODE
#define StackCtor(stack, capacity){                                               \
    StackCtor_(stack, capacity, __LINE__, __FILE__, __PRETTY_FUNCTION__, #stack);  \
}
#else
#define StackCtor(stack, capacity){                                               \
    StackCtor_(stack, capacity, 0, "", "", "");                                    \
}
#endif



#define StackDump(stack){           \
    ErrorCode stack_status = StackVerify(stack);                                    \
    StackDump_(stack, __LINE__, __FILE__, __PRETTY_FUNCTION__);                   \
}

#define PRINT_LINE printf("I'm at %s at line %d\n", __PRETTY_FUNCTION__, __LINE__);
