#include "stack.h"

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]){
    if (stack == nullptr){
        return nullptr;
    }

    if (!(StackIsEmpty(stack)      == STACK_IS_EMPTY ||
          StackIsDestructed(stack) == STACK_IS_DESTRUCTED)){
        return nullptr;
    }

    #if DEBUG_MODE & STACK_INFO
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

    #if DEBUG_MODE & CANARY_GUARD
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
        for (int i = 0; i < capacity; i++){
            stack->data[i] = POISONED_ELEM;
        }
    #endif

    stack->size     = 0;
    stack->capacity = capacity;

    stack->status   = STACK_IS_OK;

    return stack;
}

Elem_t *ResizeStack(stack_t *stack, ResizeMode mode){
    ASSERT_OK(stack);

    StackDump(stack);
    
    size_t new_capacity = 0;

    if (mode == INCREASE_CAPACITY){
        new_capacity = stack->capacity * 2;
    }
    else
    if (mode == DECREASE_CAPACITY){
        new_capacity = stack->capacity / 2;
    }
    else{
        stack->status |= STACK_RESIZE_WRONG_PARAM;
        return nullptr;
    }

    if (new_capacity < 8) new_capacity = 8;

    #if DEBUG_MODE & CANARY_GUARD
        Elem_t *try_realloc = (Elem_t *) realloc(stack->data - sizeof(u_int64_t), sizeof(Elem_t) * new_capacity + 2 * sizeof(u_int64_t));
    #else
        // printf("ptr = %p, new_capacity = %d, sizeof(Elem_t) = %d, bytes = %d\n", stack->data, new_capacity, sizeof(Elem_t), sizeof(Elem_t) * new_capacity);
        Elem_t *try_realloc = (Elem_t *) realloc(stack->data, new_capacity * sizeof(Elem_t));
    #endif

    if (try_realloc == nullptr){
        stack->status |= CANT_ALLOCATE_MEMORY;
        return nullptr;
    }

    stack->data = try_realloc;

    if (mode == INCREASE_CAPACITY){
        for (int i = (int)stack->capacity; i < (int)new_capacity; ++i){
            stack->data[i] = POISONED_ELEM;
        }
    }

    stack->capacity = new_capacity;

    ASSERT_OK(stack);

    StackDump(stack);

    return stack->data;
}

StatusCode StackPush(stack_t *stack, Elem_t value){
    if (stack == nullptr){
        StackDump(stack);
        return STACK_IS_NULLPTR;
    }

    ASSERT_OK(stack);

    if (stack->data == nullptr && stack->capacity == 0){
        #if DEBUG_MODE & CANARY_GUARD
            stack->data = (Elem_t *) calloc(START_SIZE * sizeof(Elem_t) + 2 * sizeof(u_int64_t), sizeof(char));
        #else
            stack->data = (Elem_t *) calloc(START_SIZE, sizeof(Elem_t));
        #endif
        
        // RESIZE_PLS

        if (stack->data == nullptr){
            stack->status |= CANT_ALLOCATE_MEMORY;
            StackDump(stack);
            return (StatusCode) stack->status;
        }

        for (int i = 0; i < START_SIZE; i++){
            stack->data[i] = POISONED_ELEM;
        }
        
        #if DEBUG_MODE & CANARY_GUARD
            stack->data += sizeof(u_int64_t);
        #endif

        stack->capacity = START_SIZE;
    }
    
    if (stack->size == stack->capacity){

        Elem_t *try_realloc = ResizeStack(stack, INCREASE_CAPACITY);
        if (try_realloc == nullptr){
            stack->status |= CANT_ALLOCATE_MEMORY;
            StackDump(stack);
            return (StatusCode) stack->status;
        }

    }

    stack->data[stack->size++] = value;

    ASSERT_OK(stack);

    StackDump(stack);

    return STACK_IS_OK;
}

Elem_t StackPop(stack_t *stack){
    if (stack == nullptr){
        StackDump(stack);
        return POISONED_ELEM;
    }

    ASSERT_OK(stack);

    if (stack->size == 0){
        stack->status |= STACK_IS_ALREADY_EMPTY;
        StackDump(stack);
        return POISONED_ELEM;
    }
    
    Elem_t data_elem = stack->data[stack->size];
    stack->data[stack->size--] = POISONED_ELEM;

    if (stack->size <= stack->capacity / 4){
        stack->data = ResizeStack(stack, DECREASE_CAPACITY);
        if (stack->data == nullptr){
            stack->status |= CANT_ALLOCATE_MEMORY;
            StackDump(stack);
            return POISONED_ELEM;
        }
    }

    ASSERT_OK(stack);

    StackDump(stack);

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

    while ((int)stack->size >= 0){
        stack->data[stack->size--] = POISONED_ELEM;
    }
    stack->size = 0;

    #if DEBUG_MODE & CANARY_GUARD
        free(stack->data - sizeof(u_int64_t));
    #else
        free(stack->data);
    #endif
    
    stack->data = (Elem_t *)(1000 - 7);

    stack->capacity = 0xD1ED;
    stack->size     = 0xF1FA;

    #if DEBUG_MODE & STACK_INFO
        stack->info.file = (char *)(300);
        stack->info.func = (char *)(228 - 127);
        stack->info.name = (char *)(999 - 123);
        stack->info.line = 0xEBAAL;
    #endif

    STACK_STATUS(STACK_IS_DESTRUCTED);
}

int StackVerify(stack_t *stack){
    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }

    if (StackIsEmpty(stack) == STACK_IS_EMPTY || StackIsDestructed(stack) == STACK_IS_DESTRUCTED)
        return STACK_IS_OK;
    
    int status = STACK_IS_OK;

    #if DEBUG_MODE & STACK_INFO
        if (stack->info.line == 0 || stack->info.file == nullptr || 
            stack->info.func == nullptr || stack->info.name == nullptr){
            status |= STACK_DATA_IS_RUINED | STACK_INFO_RUINED;
        }
    #endif
    
    if (stack->size > stack->capacity)
        status |= STACK_DATA_IS_RUINED | STACK_SIZE_BIGGER_THAN_CAPACITY;
    if (stack->capacity < 0)
        status |= STACK_DATA_IS_RUINED | STACK_CAPACITY_LESS_THAN_ZERO;
    if (stack->data == nullptr && stack->capacity != 0)
        status |= STACK_DATA_IS_RUINED | STACK_DATA_IS_NULLPTR;
    if (stack->size < 0)
        status |= STACK_DATA_IS_RUINED | STACK_SIZE_LESS_THAN_ZERO;

    stack->status = status; 

    return stack->status;
}

StatusCode StackIsEmpty(stack_t *stack){
    #if DEBUG_MODE & STACK_INFO
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
    #if DEBUG_MODE & STACK_INFO
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

        COS(STACK_IS_NULLPTR)

        COS(STACK_IS_DESTRUCTED)
        COS(STACK_IS_EMPTY)
        COS(DUMP_COMMITED)
        
        COS(STACK_IS_ALREADY_EMPTY)

        COS(STACK_DATA_IS_NULLPTR)
        COS(STACK_DATA_IS_RUINED)
        
        
        COS(CANT_ALLOCATE_MEMORY)

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

u_int64_t Hash(void *ptr, size_t number_of_bytes){
    int sum = 0;

    for (size_t i = 0; i < number_of_bytes; ++i){
        sum += *(char *)ptr * (number_of_bytes - i);
    }

    return sum;
}

StatusCode StackDump_(stack_t *stack, int line, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE]/*, StatusCode stack_status, char *error_msg*/){
    int stack_status = StackVerify(stack);

    if (stack_status == STACK_IS_NULLPTR){
        printf("%s\n", StackStatusPhrase(stack_status));
        return DUMP_COMMITED;
    }
    if (StackIsEmpty(stack) == STACK_IS_EMPTY)
        stack_status = STACK_IS_EMPTY;

    printf("stack<Elem_t>[%p] ", stack);

    stack_status |= StackIsEmpty(stack);
    stack_status |= StackIsDestructed(stack);
    
    int stack_has_errors = 0;

    if (stack_status & STACK_IS_EMPTY){
        printf("ok, empty(not constructed),");
    }
    else
    if (stack_status & STACK_IS_DESTRUCTED){
        printf("ok, destructed,");
    }
    else
    if (stack_status == STACK_IS_OK){
        printf("ok,");
    }
    else{
        printf("ERROR,");
        stack_has_errors = (int)'YES';
    }

    printf(" called from %s at %s \b(%d)", func, file, line);

    #if DEBUG_MODE & STACK_INFO
        if (stack_status & STACK_DATA_IS_RUINED == 0)
        printf(", \"%s\" created at %s at %s \b(%d)", stack->info.name, stack->info.func, stack->info.file, stack->info.line);
    #endif


    if (stack_has_errors){
        printf("\n");
        PRINT_ERROR(STACK_IS_ALREADY_EMPTY);
        PRINT_ERROR(CANT_ALLOCATE_MEMORY);
        PRINT_ERROR(STACK_INFO_RUINED);

        PRINT_ERROR(STACK_DATA_IS_RUINED);
        PRINT_ERROR(STACK_DATA_IS_NULLPTR);
        PRINT_ERROR(STACK_SIZE_BIGGER_THAN_CAPACITY);
        PRINT_ERROR(STACK_CAPACITY_LESS_THAN_ZERO);
        PRINT_ERROR(STACK_SIZE_LESS_THAN_ZERO);
    }
        
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
