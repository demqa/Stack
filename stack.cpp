#include "stack.h"

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]){
    if (stack == nullptr){
        return nullptr;
    }

    
    if (!(StackIsEmpty(stack)      == STACK_IS_EMPTY ||
          StackIsDestructed(stack) == STACK_IS_DESTRUCTED)){
        return nullptr;
    }


    #if DEBUG_MODE & 01
        stack->info.file = file;
        stack->info.func = func;
        stack->info.line = line_created;
        stack->info.name = stack_name;
    #endif

    if (capacity == 0){
        stack->capacity = 0;
        stack->size     = 0;
        stack->status   = STACK_IS_OK;
        stack->data     = nullptr;

        return stack;
    }

    #if DEBUG_MODE & 02
        stack->data = (Elem_t *) calloc(capacity * sizeof(Elem_t) + 2 * sizeof(u_int64_t), sizeof(char));
        if (stack->data == nullptr){
            stack->status = CANT_ALLOCATE_MEMORY;
            return stack;
        }
        stack->data += sizeof(u_int64_t);
    #else
        stack->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));
        if (stack->data == nullptr){
            stack->status = CANT_ALLOCATE_MEMORY;
            return stack;
        }
    #endif
    

    stack->size     = 0;
    stack->capacity = capacity;

    stack->status   = STACK_IS_OK;

    return stack;
}

StatusCode StackPush(stack_t *stack, Elem_t value){
    if (stack == nullptr){
        StackDump(stack);
        return STACK_IS_NULLPTR;
    }

    ASSERT_OK(stack);

    if (stack->data == nullptr && stack->capacity == 0){
        #if DEBUG_MODE & 02
            stack->data = (Elem_t *) calloc(START_SIZE * sizeof(Elem_t) + 2 * sizeof(u_int64_t), sizeof(char));
        #else
            stack->data = (Elem_t *) calloc(START_SIZE, sizeof(Elem_t));
        #endif

        if (stack->data == nullptr){
            stack->status = CANT_ALLOCATE_MEMORY;
            StackDump(stack);
            return (StatusCode) stack->status;
        }
        
        #if DEBUG_MODE & 02
            stack->data += sizeof(u_int64_t);
        #endif

        stack->capacity += START_SIZE;
    }
    
    if (stack->size + 1 == stack->capacity){

        // ResizeStack(); // hysteresis

        Elem_t *try_realloc = (Elem_t *) realloc(stack->data, sizeof(Elem_t) * (ADDITIONAL_SIZE + stack->capacity));
        if (try_realloc == nullptr){
            STACK_STATUS(CANT_ALLOCATE_MEMORY);
        }
        for (int i = stack->capacity; i < stack->capacity + ADDITIONAL_SIZE; ++i){
            stack->data[i] = POISONED_ELEM;
        }
        stack->capacity += ADDITIONAL_SIZE;

    }

    stack->data[stack->size++] = value;

    ASSERT_OK(stack);

    return STACK_IS_OK;
}

Elem_t StackPop(stack_t *stack){
    if (stack == nullptr){
        StackDump(stack);
        return POISONED_ELEM;
    }
    ASSERT_OK(stack);

    if (stack->size == 0){
        stack->status += STACK_IS_ALREADY_EMPTY;
        StackDump(stack);
        return POISONED_ELEM;
    }
    
    Elem_t data_elem = stack->data[stack->size--];
    stack->data[stack->size + 1] = POISONED_ELEM;

    ASSERT_OK(stack);

    return data_elem;
}

StatusCode StackDtor(stack_t *stack){
    if (stack == nullptr){
        StackDump(stack);
        return STACK_IS_NULLPTR;
    }

    
    if (StackIsDestructed(stack) == STACK_IS_DESTRUCTED){
        StackDump(stack);
        return STACK_IS_DESTRUCTED;
    }

    if (stack->data == nullptr){
        if (stack->capacity == 0){
            stack->capacity = 0xBEBA;
            stack->size     = 0xDEDA;
            STACK_STATUS(STACK_IS_DESTRUCTED);
        }
        StackDump(stack);
        STACK_STATUS(STACK_DATA_IS_NULLPTR)
    }
    PRINT_LINE;

    while ((int)stack->size >= 0){
        printf("stack->size = %d\n", stack->size);
        stack->data[stack->size--] = POISONED_ELEM;
    }
    stack->size = 0;
    PRINT_LINE;

    #if DEBUG_MODE & 02
        free(stack->data - sizeof(u_int64_t));
    #else
        free(stack->data);
    #endif
    PRINT_LINE;

    stack->data = (Elem_t *)(1000 - 7);

    stack->capacity = 0xD1ED;
    stack->size     = 0xF1FA;

    PRINT_LINE;

    #if DEBUG_MODE & 01
        stack->info.file = (char *)(300);
        stack->info.func = (char *)(228 - 127);
        stack->info.name = (char *)(999 - 123);
        stack->info.line = 0xEBAAL;
    #endif

    PRINT_LINE;

    STACK_STATUS(STACK_IS_DESTRUCTED);
}


int StackVerify(stack_t *stack){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }

    StatusCode status = STACK_IS_OK;

    #if DEBUG_MODE & 01
        // check StackInfo()
    #endif
    
    if (stack->size > stack->capacity) return STACK_SIZE_BIGGER_THAN_CAPACITY + STACK_DATA_IS_RUINED;
    if (stack->data == nullptr && stack->capacity != 0 && stack->size != 0) return STACK_DATA_IS_NULLPTR + STACK_DATA_IS_RUINED;
    if (stack->capacity < 0) return STACK_CAPACITY_LESS_THAN_ZERO + STACK_DATA_IS_RUINED;
    if (stack->size < 0) return STACK_SIZE_LESS_THAN_ZERO + STACK_DATA_IS_RUINED;
    

    stack->status = status; 

    return stack->status;
}

StatusCode StackIsEmpty(stack_t *stack){
    #if DEBUG_MODE & 01
        if (stack->data == nullptr && stack->size == 0 && stack->capacity == 0 && stack->status == STACK_IS_OK &&
            stack->info.line == 0 && stack->info.file == nullptr && stack->info.func == nullptr && stack->info.name == nullptr){
            return STACK_IS_EMPTY;
        }
    #else
        if (stack->data == nullptr && stack->size == 0 && stack->capacity == 0 && stack->status == STACK_IS_OK){
            return STACK_IS_EMPTY;
        }
    #endif
    return RESULT_IS_UNKNOWN;
}

StatusCode StackIsDestructed(stack_t *stack){
    #if DEBUG_MODE & 01
        if (((stack->data == (Elem_t *)(1000 - 7) && stack->capacity == 0xD1ED && stack->size == 0xF1FA) ||
             (stack->data == nullptr              && stack->capacity == 0xBEBA && stack->size == 0xDEDA)) &&
              stack->info.file == (char *)(300)       && stack->info.func == (char *)(228 - 127) &&
              stack->info.name == (char *)(999 - 123) && stack->info.line == 0xEBAAL){
            return STACK_IS_DESTRUCTED;
        }
    #else
        if ((stack->data == (Elem_t *)(1000 - 7) && stack->capacity == 0xD1ED && stack->size == 0xF1FA ||
             stack->data == nullptr              && stack->capacity == 0xBEBA && stack->size == 0xDEDA)){
            return STACK_IS_DESTRUCTED;
        }
    #endif
    return RESULT_IS_UNKNOWN;
}


StatusCode CheckError(stack_t *stack){
    if (stack == nullptr){
        printf("STACK IS NULLPTR, ARE U CRAZY?\n");
        return STACK_IS_NULLPTR;
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

        case_of_switch(STACK_IS_NULLPTR)

        case_of_switch(STACK_IS_DESTRUCTED)
        case_of_switch(STACK_IS_EMPTY)
        case_of_switch(DUMP_COMMITED)
        
        case_of_switch(STACK_IS_ALREADY_EMPTY)

        case_of_switch(STACK_DATA_IS_NULLPTR)
        case_of_switch(STACK_DATA_IS_RUINED)
        
        
        case_of_switch(CANT_ALLOCATE_MEMORY)

        default: return "UNDEFINED_ERROR";
    }
}

void PrintElem_t(Elem_t *value){
    printf("{");
    if (value->num == POISONED_ELEM.num){
        printf("POISON(%d)", value->num);
    }
    else{
        printf("num = %d", value->num);
    }
    printf("}\n");
}


int NumberOfCharacters(int edge){
    int num = 1;
    for (int p = 1; p * 10 <= edge; p *= 10, ++num);
    return num;
}


StatusCode StackDump_(stack_t *stack, int line, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE]/*, StatusCode stack_status, char *error_msg*/){
    int stack_status = StackVerify(stack);

    if (stack_status == STACK_IS_NULLPTR){
        printf("stack<Elem_t>[%p]\n", nullptr);
        return DUMP_COMMITED;
    }
    // PRINT_LINE;

    printf("stack<Elem_t>[%p] ", stack);

    stack_status |= StackIsEmpty(stack);
    stack_status |= StackIsDestructed(stack);

    // PRINT_LINE;

    if (stack_status & STACK_IS_EMPTY){
        printf("ok, empty(not constructed), ");
    }
    else
    if (stack_status & STACK_IS_DESTRUCTED){
        PRINT_LINE;
        printf("ok, destructed,");
    }
    else
    if (stack_status == STACK_IS_OK){
        printf("ok,");
    }
    else{
        PRINT_LINE;
        printf("ERROR: ");
        PRINT_ERROR(STACK_IS_ALREADY_EMPTY);
        PRINT_ERROR(CANT_ALLOCATE_MEMORY);

        PRINT_ERROR(STACK_DATA_IS_RUINED);
        PRINT_ERROR(STACK_DATA_IS_NULLPTR);
        PRINT_ERROR(STACK_SIZE_BIGGER_THAN_CAPACITY);
        PRINT_ERROR(STACK_CAPACITY_LESS_THAN_ZERO);
        PRINT_ERROR(STACK_SIZE_LESS_THAN_ZERO);

        // PRINT_ERROR();
    }

    // PRINT_LINE;

    printf(" called from %s at %s \b(%d)", func, file, line);

    // PRINT_LINE;

    #if DEBUG_MODE & 01
        if (stack_status & (STACK_IS_EMPTY | STACK_IS_DESTRUCTED) == 0)
            printf(", \"%s\" created at %s at %s \b(%d)", stack->info.name, stack->info.func, stack->info.file, stack->info.line);
    #endif
    printf("\n");

    printf("    {\n");

    // PRINT_LINE;

    printf("    size = %d\n",     (int)stack->size);
    printf("    capacity = %d\n", (int)stack->capacity);

    printf("    data[%p]\n", stack->data);

    if (stack_status == STACK_IS_OK){
        printf("        {\n");
        for (int i = 0; i < stack->capacity; i++){
            printf("         data[%*d] = ", NumberOfCharacters((int)stack->capacity - 1), i);
            PrintElem_t(stack->data + i);
        }
        printf("        }\n");
    }


    printf("    }\n");

    printf("........Dumped...\n\n");
    
    // commented because of empty stack will not be empty anymore...
    // stack->status += DUMP_COMMITED;

    return DUMP_COMMITED;
}
