#include "stack.h"

///////////////////////////////////////////////////////////////////
////////////////////         SOMEHOW          /////////////////////
////// HAVE TO CHECK REPEAT CALL NOT TO ALLOC MEMORY AGAIN ////////
///////////////////////////////////////////////////////////////////

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]){
    if (stack == nullptr){
        return nullptr;
    }

    StatusCode stack_status = StackVerify(stack);
    if (!(stack_status == STACK_IS_EMPTY || stack_status == STACK_IS_DESTRUCTED)){
        return nullptr;
    }


    // printf("line = %d, file = %s, func = %s, name = %s\n", line_created, file, func, stack_name);
    #ifdef DEBUG_MODE
        stack->info.file = file;
        stack->info.func = func;
        stack->info.line = line_created;
        stack->info.name = stack_name;
    #endif

    if (capacity == 0){
        stack->capacity = 0;
        stack->size     = 0;
        stack->status    = STACK_IS_OK;
        stack->data     = nullptr;

        return stack;
    }

    stack->data = (Elem_t *) calloc(capacity, sizeof(int));
    if (stack->data == nullptr){
        stack->status = CANT_ALLOCATE_MEMORY;
        return stack;
    }

    stack->size     = 0;
    stack->capacity = capacity;

    stack->status    = STACK_IS_OK;

    return stack;
}

StatusCode StackPush(stack_t *stack, Elem_t value){
    if (stack == nullptr){
        StackDump(stack);
        return POISON;
    }

    ASSERT_OK(stack);
    // VERIFIED THAT STACK IS NORMAL

    if (stack->data == nullptr && stack->capacity == 0){
        stack->data = (Elem_t *) calloc(ADDITIONAL_SIZE, sizeof(int));
        if (stack->data == nullptr){
            STACK_ERROR(CANT_ALLOCATE_MEMORY)
        }
    
        stack->capacity += ADDITIONAL_SIZE;
    }
    
    if (stack->size + 1 == stack->capacity){

        // ResizeStack(); // hysteresis

        Elem_t *try_realloc = (Elem_t *) realloc(stack->data, sizeof(Elem_t) * (ADDITIONAL_SIZE + stack->capacity));
        if (try_realloc == nullptr){
            STACK_ERROR(CANT_ALLOCATE_MEMORY);
        }
        stack->capacity += ADDITIONAL_SIZE;

    }

    stack->data[stack->size++] = value;

    ASSERT_OK(stack);

    return STACK_IS_OK;
}

Elem_t StackPop(stack_t *stack){
    if (stack == nullptr){
        // DUMP
        // DUMP
        return POISONED_ELEM;
    }
    ASSERT_OK(stack);

    if (stack->size == 0){
        stack->status = STACK_IS_ALREADY_EMPTY;
        return {};
    }
    
    Elem_t data_elem = stack->data[--stack->size];
    stack->data[stack->size + 1] = POISONED_ELEM;

    ASSERT_OK(stack);

    return data_elem;
}

StatusCode StackDtor(stack_t *stack){
    if (stack == nullptr){
        return POISON;
    }

    StatusCode stack_status = StackVerify(stack);
    if (stack_status == STACK_IS_DESTRUCTED){
        return STACK_IS_DESTRUCTED;
    }
    else
        if (stack_status != STACK_IS_OK){
            return stack_status;
    }

    if (stack->data == nullptr){
        if (stack->capacity == 0){
            stack->capacity = 0xBEBA; 
            stack->size     = 0xDEDA;
            return STACK_IS_OK;
        }
        STACK_ERROR(STACK_DATA_IS_NULLPTR)
    }


    while (stack->size > 0){
        stack->data[stack->size] = POISONED_ELEM;
    }

    free(stack->data);

    stack->data = (Elem_t *)(1000 - 7);

    stack->capacity = 0xD1ED;
    stack->size     = 0xF1FA;

    stack->status = STACK_IS_DESTRUCTED;

    return stack->status;
}


StatusCode StackVerify(stack_t *stack){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }

    if (stack->status != STACK_IS_OK){
        return stack->status;
    }

    if ((stack->data == (Elem_t *)(1000 - 7) && stack->capacity == 0xD1ED && stack->size == 0xF1FA ||
         stack->data == nullptr              && stack->capacity == 0xBEBA && stack->size == 0xDEDA)){
        return STACK_IS_DESTRUCTED;
    }

    // checking on empty
    #ifdef DEBUG_MODE
        if (stack->data == nullptr && stack->size == 0 && stack->capacity == 0 && stack->status == STACK_IS_OK &&
            stack->info.line == 0 && stack->info.file == nullptr && stack->info.func == nullptr && stack->info.name == nullptr){
            return STACK_IS_EMPTY;
        }
    #else
        if (stack->data == nullptr && stack->size == 0 && stack->capacity == 0 && stack->status == STACK_IS_OK){
            return STACK_IS_EMPTY;
        }
    #endif
    
    if (stack->size > stack->capacity || 
        stack->data == nullptr && stack->capacity != 0 && stack->size != 0 || 
        stack->capacity < 0 ||
        stack->size < 0){
        return STACK_DATA_IS_RUINED;
    }

    return stack->status;
}


StatusCode CheckError(stack_t *stack){
    if (stack == nullptr){
        return POISON;
    }

    if (stack->status){
        const char *phrase = StackStatusPhrase(stack->status);
        assert(phrase != nullptr);

        // LOG FILE

        printf("error №%x, %s\n", stack->status, phrase);

        stack->status = STACK_IS_OK;
    }

    return STACK_IS_OK;
}

const char *StackStatusPhrase(int error_code){
    switch (error_code){
        case 0: return "OK";

        case_of_switch(STACK_IS_DESTRUCTED)
        case_of_switch(DUMP_COMMITED)
        case_of_switch(STACK_IS_EMPTY)


        case_of_switch(STACK_IS_NULLPTR)
        case_of_switch(STACK_IS_ALREADY_EMPTY)

        case_of_switch(STACK_DATA_IS_NULLPTR)
        case_of_switch(STACK_DATA_IS_RUINED)
        
        
        case_of_switch(CANT_ALLOCATE_MEMORY)

        default: return "UNDEFINED_ERROR";
    }
}

StatusCode StackDump_(stack_t *stack, int line, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE]/*, StatusCode stack_status, char *error_msg*/){
    StatusCode stack_status = StackVerify(stack);

    if (stack_status == STACK_IS_NULLPTR){
        printf("stack<Elem_t>[%p]\n", nullptr);
        return DUMP_COMMITED;
    }

    printf("stack<Elem_t>[%p]", stack);
    switch (stack_status){
        case STACK_IS_OK:
            printf(" ERROR %x %s", stack_status, StackStatusPhrase(stack_status));
            break;
        case STACK_IS_EMPTY:
            printf(" ok, empty(not constructed) %x %s", stack_status, StackStatusPhrase(stack_status));
            return DUMP_COMMITED;
            break;
        case STACK_IS_DESTRUCTED:
            printf(" ok, %x %s", stack_status, StackStatusPhrase(stack_status));
            break;
        default:
            printf(" ok, called from %s at %s \b(%d) \"%s\" at %s at %s \b(%d)\n", func, file, line, stack->info.name, stack->info.func, stack->info.file, stack->info.line);
            break;
    }
    if (stack_status != STACK_IS_OK){
        
    }else{
        

    }

    stack->status = DUMP_COMMITED;
    return DUMP_COMMITED;
}
