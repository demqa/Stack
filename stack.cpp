#include "stack.h"

///////////////////////////////////////////////////////////////////
////////////////////         SOMEHOW          /////////////////////
////// HAVE TO CHECK REPEAT CALL NOT TO ALLOC MEMORY AGAIN ////////
///////////////////////////////////////////////////////////////////

stack_t *StackCtor(stack_t *stack, size_t capacity = 0){
    if (stack == nullptr){
        // dump
        return nullptr;
    }

    //  check on empty or destructed stack
    //  else exit

    if (capacity == 0){
        stack->capacity = 0;
        stack->size     = 0;
        stack->error    = 0;
        stack->data     = nullptr;

        return stack;
    }

    stack->data = (int *) calloc(capacity, sizeof(int));
    if (stack->data == nullptr){
        STACK_ERROR(CANT_ALLOCATE_MEMORY)
    }

    stack->size     = 0;
    stack->capacity = capacity;

    stack->error    = 0;

    return stack;
}




int StackPop(stack_t *stack){
    if (stack == nullptr){
        // DUMP
        return POISON;
    }
    StackVerify(stack);

    if (stack->size == 0){
        STACK_ERROR(STACK_IS_ALREADY_EMPTY)
    }
    
    int data_elem = stack->data[stack->size--];
    stack->data[stack->size + 1] = 0x1341;

    StackVerify(stack);

    return data_elem;
}



int StackPush(stack_t *stack, int elem){
    if (stack == nullptr){
        // DUMP
        return POISON;
    }

    StackVerify(stack);
    // VERIFIED THAT STACK IS NORMAL

    if (stack->data == nullptr && stack->capacity == 0){
        stack->data = (int *) calloc(ADDITIONAL_SIZE, sizeof(int));
        if (stack->data == nullptr){
            STACK_ERROR(CANT_ALLOCATE_MEMORY)
        }
    
        stack->capacity += ADDITIONAL_SIZE;
    }
    
    if (stack->size + 1 == stack->capacity){

        // ResizeStack(); // hysteresis

        int *try_realloc = (int *) realloc(stack->data, sizeof(int) * (ADDITIONAL_SIZE + stack->capacity));
        if (try_realloc == nullptr){
            STACK_ERROR(CANT_ALLOCATE_MEMORY);
        }
        stack->capacity += ADDITIONAL_SIZE;

    }

    stack->data[stack->size++] = elem;

    StackVerify(stack);

    return 0;
}

int StackDtor(stack_t *stack){
    if (stack == nullptr){
        // LOG
        // DUMP
        return POISON;
    }
    StackVerify(stack);

    if (stack->data == nullptr){
        if (stack->capacity == 0){
            stack->capacity = 0xBEBA; 
            stack->size     = 0xDEDA;
            return 0;
        }
        STACK_ERROR(STACK_DATA_IS_NULLPTR)
    }


    while (stack->size > 0){
        stack->data[stack->size] = stack->size-- * 0x707A;
    }

    free(stack->data);
    stack->data = nullptr;

    stack->capacity = 0xD1ED;
    stack->size     = 0xF1FA;

    stack->error = 0;

    return 0;
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




int StackVerify(stack_t *stack){ // TODO && USE THIS SOMEHOW

    if (stack->error){
        return stack->error;
    }

    if (stack == nullptr){
        return STACK_IS_NULLPTR;
    }
    
    if (stack->size > stack->capacity || stack->data == nullptr && stack->capacity != 0){
        return STACK_DATA_IS_RUINED;
    }

    stack->error = 0;

    return stack->error;
}

///
    ////
         ////
              ////
                   ////
                        ////
/// //// //// //// //// //// //// //// //// //// ////
/// New function                  ////           ////
/// Abort if STACK IS NULLPTR          ////      ////
/// (My assert)                             //// ////
/// //// //// //// //// //// //// //// //// //// ////
                                                     ////
                                                         ////
                                                             ////
/////////////////////////////////////////////////////////////////////
////// NOT FUNCTION, A MACRO //////////////////////////////////////////////
/////////////////////////////////////////WANNA GIRL/////////////////////////////
//////////1000-7/////////////////////////////////////////////////////////////////////
//////???????????????????????/////////////////////////////////
///////////////////////////////////////


int CheckError(stack_t *stack){
    if (stack == nullptr){
        // MY_MACRO
        // LOG_FILE
        // __FILE__
        // __LINE__ 
        // __FUNC__
        // ??????
        return POISON;
    }

    if (stack->error){
        const char *phrase = ErrorCodePhrase(stack->error);
        assert(phrase != nullptr);

        // LOG FILE

        printf("error №%x, %s\n", stack->error, phrase);

        stack->error = 0;
    }

    return 0;
}

