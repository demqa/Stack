#include "stack.h"

///////////////////////////////////////////////////////////////////
////////////////////         SOMEHOW          /////////////////////
////// HAVE TO CHECK REPEAT CALL NOT TO ALLOC MEMORY AGAIN ////////
///////////////////////////////////////////////////////////////////

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]){
    if (stack == nullptr){
        return nullptr;
    }

    //  check on empty or destructed stack
    //  else exit

    if (capacity == 0){
        stack->capacity = 0;
        stack->size     = 0;
        stack->error    = STACK_IS_OK;
        stack->data     = nullptr;

        return stack;
    }

    stack->data = (Elem_t *) calloc(capacity, sizeof(int));
    if (stack->data == nullptr){
        stack->error = CANT_ALLOCATE_MEMORY;
        return stack;
    }

    stack->size     = 0;
    stack->capacity = capacity;

    stack->error    = STACK_IS_OK;

    return stack;
}

ErrorCode StackPush(stack_t *stack, Elem_t value){
    if (stack == nullptr){
        // DUMP
        return POISON;
    }

    StackVerify(stack);
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

        Elem_t *try_realloc = (Elem_t *) realloc(stack->data, sizeof(int) * (ADDITIONAL_SIZE + stack->capacity));
        if (try_realloc == nullptr){
            STACK_ERROR(CANT_ALLOCATE_MEMORY);
        }
        stack->capacity += ADDITIONAL_SIZE;

    }

    stack->data[stack->size++] = value;

    StackVerify(stack);

    return STACK_IS_OK;
}

Elem_t StackPop(stack_t *stack){
    if (stack == nullptr){
        // DUMP
        return {POISON};
    }
    StackVerify(stack);

    if (stack->size == 0){
        // STACK_ERROR(STACK_IS_ALREADY_EMPTY)
        return {};
    }
    
    Elem_t data_elem = stack->data[stack->size--];
    stack->data[stack->size + 1] = {};

    StackVerify(stack);

    return data_elem;
}

ErrorCode StackDtor(stack_t *stack){
    if (stack == nullptr){
        return POISON;
    }

    ErrorCode stack_status = StackVerify(stack);
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
        stack->data[stack->size] = {};
    }

    free(stack->data);

    stack->data = (Elem_t *)(1000 - 7);

    stack->capacity = 0xD1ED;
    stack->size     = 0xF1FA;

    stack->error = STACK_IS_DESTRUCTED;

    return stack->error;
}



ErrorCode StackVerify(stack_t *stack){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }

    if ((stack->data == (Elem_t *)(1000 - 7) && stack->capacity == 0xD1ED && stack->size == 0xF1FA ||
         stack->data == nullptr              && stack->capacity == 0xBEBA && stack->size == 0xDEDA)){
        return STACK_IS_DESTRUCTED;
    }

    // checking on empty
    #ifdef DEBUG_MODE
        if (stack->data == nullptr && stack->size == 0 && stack->capacity == 0 && stack->error == STACK_IS_OK &&
            stack->info.line == 0 && stack->info.file[0] == '\0' && stack->info.func[0] == '\0' && stack->info.name[0] == '\0'){
            return STACK_IS_EMPTY;
        }
    #else
        if (stack->data == nullptr && stack->size == 0 && stack->capacity == 0 && stack->error == STACK_IS_OK){
            return STACK_IS_EMPTY;
        }
    #endif

    if (stack->error){
        return stack->error;
    }
    
    if (stack->size > stack->capacity || 
        stack->data == nullptr && stack->capacity != 0 && stack->size != 0 || 
        stack->capacity < 0 ||
        stack->size < 0){
        return STACK_DATA_IS_RUINED;
    }

    return stack->error;
}


ErrorCode CheckError(stack_t *stack){
    if (stack == nullptr){
        return POISON;
    }

    if (stack->error){
        const char *phrase = ErrorCodePhrase(stack->error);
        assert(phrase != nullptr);

        // LOG FILE

        printf("error №%x, %s\n", stack->error, phrase);

        stack->error = STACK_IS_OK;
    }

    return STACK_IS_OK;
}

const char *ErrorCodePhrase(int error_code){
    switch (error_code){
        case 0: return "OK";

        case_of_switch(STACK_IS_NULLPTR)
        case_of_switch(STACK_WITH_ZERO_ELEMS)
        case_of_switch(STACK_IS_ALREADY_EMPTY)

        case_of_switch(STACK_DATA_IS_NULLPTR)
        case_of_switch(STACK_DATA_IS_RUINED)
        
        case_of_switch(CANT_ALLOCATE_MEMORY)

        default: return "UNDEFINED_ERROR";
    }
}