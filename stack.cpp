#include "stack.h"

#if DEBUG_MODE & HASH_GUARD

    u_int64_t Hash(void *memory, size_t number_of_bytes){
        u_int64_t sum = 0;

        for (size_t i = 0; i < number_of_bytes; ++i){
            sum += (*((char *)memory + i) + 300) * (number_of_bytes * i);
        }

        return sum;
    }

    u_int64_t CalculateHash(stack_t *stack){
        if (stack == nullptr){
            return (u_int64_t)STACK_IS_NULLPTR;
        }

        u_int64_t this_hash_was_in_stack = stack->hash;
        stack->hash = 0;

        u_int64_t hash = Hash((void *)stack, sizeof(stack));

        if (stack->data != nullptr || StackIsDestructed(stack) != STACK_IS_DESTRUCTED){
            hash += Hash((void *)stack->data, stack->capacity * sizeof(Elem_t));
        }

        hash += ADDRESS(stack->data, Elem_t);
        stack->hash = this_hash_was_in_stack;
        
        return hash;
    }

#endif

stack_t *StackCtor_(stack_t *stack, size_t capacity, int line_created, const char file[STRING_MAX_SIZE], const char func[STRING_MAX_SIZE], const char stack_name[STRING_MAX_SIZE]){
    if (stack == nullptr){
        return nullptr;
    }

    if (StackIsEmpty(stack) != STACK_IS_EMPTY &&
        StackIsDestructed(stack) != STACK_IS_DESTRUCTED

        #if DEBUG_MODE & STACK_INFO
        &&
        StackInfoIsEmpty(stack)
        #endif

        ){
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

    if (capacity & 07 != 0) capacity = (capacity / 8 + 1) * 8;

    #if DEBUG_MODE & HIPPO_GUARD
        stack->HIPPO = HIPPO ^ ADDRESS(stack, stack_t);
        stack->POTAM = POTAM ^ ADDRESS(stack, stack_t);
    
        void *ptr = nullptr;
        ptr = (void *) calloc(capacity * sizeof(Elem_t) + 2 * sizeof(u_int64_t), 1);

        if (ptr == nullptr){
            stack->status = CANT_ALLOCATE_MEMORY;
            return stack;
        }
        
        *((u_int64_t *)ptr) = stack->HIPPO;
        ptr = ptr + sizeof(u_int64_t) + sizeof(Elem_t) * capacity;

        *((u_int64_t *)ptr) = stack->POTAM;
        ptr = ptr - sizeof(Elem_t) * capacity;
        
        stack->data = (Elem_t *) ptr;
    #else
        stack->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));
        if (stack->data == nullptr){
            stack->status = CANT_ALLOCATE_MEMORY;
            return stack;
        }
    #endif

    for (int i = 0; i < capacity; i++){
        stack->data[i] = POISONED_ELEM;
    }

    stack->size     = 0;
    stack->capacity = capacity;

    stack->status   = STACK_IS_OK;

    #if DEBUG_MODE & HASH_GUARD
        stack->hash = CalculateHash(stack);
    #endif

    ASSERT_OK(stack);

    return stack;
}

Elem_t *ResizeStack(stack_t *stack, ResizeMode mode){
    ASSERT_OK(stack);

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
        StackDump(stack);
        return nullptr;
    }

    if (new_capacity < 8) new_capacity = 8;

    #if DEBUG_MODE & HIPPO_GUARD
        void *try_realloc = (void *) realloc((void *)stack->data - sizeof(u_int64_t), sizeof(Elem_t) * new_capacity + 2 * sizeof(u_int64_t));
    #else
        void *try_realloc = (void *) realloc(stack->data, new_capacity * sizeof(Elem_t));
    #endif

    if (try_realloc == nullptr){
        stack->status |= CANT_ALLOCATE_MEMORY;
        StackDump(stack);
        return nullptr;
    }

    #if DEBUG_MODE & HIPPO_GUARD
        try_realloc = try_realloc + sizeof(u_int64_t);
    #endif

    stack->data = (Elem_t *) try_realloc;

    if (mode == INCREASE_CAPACITY){
        for (int i = (int)stack->capacity; i < (int)new_capacity; ++i){
            stack->data[i] = POISONED_ELEM;
        }
    }

    stack->capacity = new_capacity;

    #if DEBUG_MODE & HASH_GUARD
        stack->hash = CalculateHash(stack);
    #endif

    ASSERT_OK(stack);

    return stack->data;
}

StatusCode StackPush(stack_t *stack, Elem_t value){
    if (stack == nullptr){
        StackDump(stack);
        return STACK_IS_NULLPTR;
    }

    ASSERT_OK(stack);

    if (stack->data == nullptr && stack->capacity == 0 ||
        stack->size == stack->capacity){

        Elem_t *try_realloc = ResizeStack(stack, INCREASE_CAPACITY);
        if (try_realloc == nullptr){
            stack->status |= CANT_ALLOCATE_MEMORY;
            StackDump(stack);
            return (StatusCode) stack->status;
        }
    }

    stack->data[stack->size++] = value;

    #if DEBUG_MODE & HASH_GUARD
        stack->hash = CalculateHash(stack);
    #endif

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

    #if DEBUG_MODE & HASH_GUARD
        stack->hash = CalculateHash(stack);
    #endif

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
        STACK_STATUS(STACK_DATA_IS_NULLPTR);
    }

    while ((int)stack->size >= 0){
        stack->data[stack->size--] = POISONED_ELEM;
    }
    stack->size = 0;

    #if DEBUG_MODE & HIPPO_GUARD
        free((void *)stack->data - sizeof(u_int64_t));
    #else
        free(stack->data);
    #endif
    
    stack->data = (Elem_t *)(1000 - 7);

    stack->capacity = 0xD1ED;
    stack->size     = 0xF1FA;

    #if DEBUG_MODE & HIPPO_GUARD
        stack->HIPPO = 0xDEADF1DC ^ ADDRESS(stack, stack_t);
        stack->POTAM = 0xAC47AC47 ^ ADDRESS(stack, stack_t);
    #endif

    #if DEBUG_MODE & STACK_INFO
        stack->info.file = (char *)(300);
        stack->info.func = (char *)(228 - 127);
        stack->info.name = (char *)(999 - 123);
        stack->info.line = 0xEBAAL;
    #endif

    #if DEBUG_MODE & HASH_GUARD
        stack->hash = 0xFA1EBACAUL;
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
        if (stack->info.line == 0       || stack->info.file == nullptr || 
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

    #if DEBUG_MODE & HIPPO_GUARD
        if (stack->HIPPO != (u_int64_t) (HIPPO ^ ADDRESS(stack, stack_t)))
            status |= STACK_DATA_IS_RUINED | STACK_LEFT_HIPPO_RUINED;
        if (stack->POTAM != (u_int64_t) (POTAM ^ ADDRESS(stack, stack_t)))
            status |= STACK_DATA_IS_RUINED | STACK_RIGHT_POTAM_RUINED;
            

        if (stack->data != nullptr && status & STACK_DATA_IS_RUINED == 0 && status & STACK_IS_DESTRUCTED == 0){
            void *ptr = (void *) stack->data - sizeof(u_int64_t);
            
            if (*((u_int64_t *)ptr) != HIPPO ^ ADDRESS(stack, stack_t))
                status |= STACK_DATA_IS_RUINED | DATA_LEFT_HIPPO_RUINED;

            ptr = ptr + sizeof(u_int64_t) + sizeof(Elem_t) * stack->capacity;

            if (*((u_int64_t *)ptr) != POTAM ^ ADDRESS(stack, stack_t))
                status |= STACK_DATA_IS_RUINED | DATA_RIGHT_POTAM_RUINED;

            ptr = ptr - sizeof(Elem_t) * stack->capacity;
        
            stack->data = (Elem_t *) ptr;
        }
    #endif

    #if DEBUG_MODE & HASH_GUARD
        if (stack->hash != CalculateHash(stack)){
            status |= STACK_DATA_IS_RUINED | STACK_HASH_RUINED;
        }
    #endif

    stack->status |= status; 

    return status;
}

StatusCode StackIsEmpty(stack_t *stack){
    int stack_is_empty = 1;

    if (stack->data != nullptr || stack->size != 0 || stack->capacity != 0 || stack->status != STACK_IS_OK){
        stack_is_empty = 0;
    }

    #if DEBUG_MODE & STACK_INFO
        if (!StackInfoIsEmpty(stack)){
            stack_is_empty = 0;
        }
    #endif

    #if DEBUG_MODE & HIPPO_GUARD
        if (stack->HIPPO != 0 || stack->POTAM != 0){
            stack_is_empty = 0;
        }
    #endif

    #if DEBUG_MODE & HASH_GUARD
        if (stack->hash != 0){
            stack_is_empty = 0;
        }
    #endif

    if (stack_is_empty)
        return STACK_IS_EMPTY;
    else
        return RESULT_IS_UNKNOWN;

}

#if DEBUG_MODE & STACK_INFO
    int StackInfoIsEmpty(stack_t *stack){
        return stack->info.line == 0 && stack->info.file == nullptr && stack->info.func == nullptr && stack->info.name == nullptr;
    }
#endif

StatusCode StackIsDestructed(stack_t *stack){
    if (((stack->data == (Elem_t *)(1000 - 7) && stack->capacity == 0xD1ED && stack->size == 0xF1FA) ||
         (stack->data == nullptr              && stack->capacity == 0xBEBA && stack->size == 0xDEDA))
                
        #if DEBUG_MODE & STACK_INFO
        &&
          stack->info.file == (char *)(300)       && stack->info.func == (char *)(228 - 127) &&
          stack->info.name == (char *)(999 - 123) && stack->info.line == 0xEBAAL
        #endif

        #if DEBUG_MODE & HIPPO_GUARD
        &&
          stack->HIPPO == 0xDEADF1DC ^ ADDRESS(stack, stack_t) && 
          stack->POTAM == 0xAC47AC47 ^ ADDRESS(stack, stack_t)
        #endif

        #if DEBUG_MODE & HASH_GUARD
        &&
          stack->hash == 0xFA1EBACAUL
        #endif

        ){
        return STACK_IS_DESTRUCTED;
    }

    return RESULT_IS_UNKNOWN;
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
    static int r = 0;
    if (r++ == 0){
        printf(".....$*$*\n"
               "...$*....$*............$*.\n"
               "..$*.......$*.......$*....$*\n"
               ".$*.........$*....$*.......$*\n"
               ".$*...........$*.$*........$*\n"
               ".$*.............*.........$*\n"
               "..$*....................$*\n"
               "...$*.................$*\n"
               "... $*..............$*\n"
               ".....$*...........$*\n"
               "......$*........$*\n"
               "........$*....$*\n"
               ".........$*.$*\n"
               "..........*$*\n"
               "...........$\n"
               "...........*\n"
               "...........$\n"
               "...........*\n\n");
    }

    int stack_status = StackVerify(stack);

    if (stack_status == STACK_IS_NULLPTR){
        printf("STACK_IS_NULLPTR\n");
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

    stack_status |= stack->status;

    if (stack_has_errors){
        printf("\n");
        PRINT_ERROR(STACK_IS_ALREADY_EMPTY)
        PRINT_ERROR(CANT_ALLOCATE_MEMORY)

        PRINT_ERROR(STACK_DATA_IS_RUINED)

        PRINT_ERROR(STACK_DATA_IS_NULLPTR)
        PRINT_ERROR(STACK_SIZE_BIGGER_THAN_CAPACITY)
        PRINT_ERROR(STACK_CAPACITY_LESS_THAN_ZERO)
        PRINT_ERROR(STACK_SIZE_LESS_THAN_ZERO)
        PRINT_ERROR(STACK_RESIZE_WRONG_PARAM)

    #if DEBUG_MODE & STACK_INFO
        PRINT_ERROR(STACK_INFO_RUINED)
    #endif

    #if DEBUG_MODE & HIPPO_GUARD
        PRINT_ERROR(STACK_LEFT_HIPPO_RUINED)
        PRINT_ERROR(STACK_RIGHT_POTAM_RUINED)

        PRINT_ERROR(DATA_LEFT_HIPPO_RUINED)
        PRINT_ERROR(DATA_RIGHT_POTAM_RUINED)
    #endif

    #if DEBUG_MODE & HASH_GUARD
        PRINT_ERROR(STACK_HASH_RUINED)
    #endif
    }
        
    printf("\n");

    printf("    {\n");
    printf("    size = %d\n",     (int)stack->size);
    printf("    capacity = %d\n", (int)stack->capacity);
    #if DEBUG_MODE & HASH_GUARD
        printf("    hash = %x\n",     (int)stack->hash);
    #endif
    printf("    data[%p]\n",           stack->data);

    if (stack_status == STACK_IS_OK){
        printf("        {\n");
        for (int i = 0; i < stack->capacity; i++){
            printf("         data[%*d] = ", NumberOfCharacters((int)stack->capacity - 1), i);
            PrintElem_t(stack->data + i);
        }
        printf("        }\n");
    }

    printf("    }\n");

    printf("_________________DDDuMpEDDD_________________\n\n");

    return DUMP_COMMITED;
}
