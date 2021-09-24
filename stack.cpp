#include "stack.h"

///////////////////////////////////////////////////////////////////
////////////////////         SOMEHOW          /////////////////////
////// HAVE TO CHECK REPEAT CALL NOT TO ALLOC MEMORY AGAIN ////////
///////////////////////////////////////////////////////////////////

stack_t *ConstructStack(stack_t *stack, size_t capacity = 0){
    if (stack == nullptr){
        // dump
        return nullptr;
    }

    if (capacity == 0){
        stack->capacity = 0;
        stack->size     = 0;
        stack->error    = 0;
        stack->top      = nullptr;

        return stack;
    }

    stack->top = (int *) calloc(capacity, sizeof(int));
    if (stack->top == nullptr){
        STACK_ERROR(CANT_ALLOCATE_MEMORY)
    }

    stack->size  = 0;
    stack->capacity = capacity;

    stack->error = 0;
    return stack;
}

// int Pop(stack_t *stack){
//     if (stack == nullptr){
//         return POISON;
//     }
//     if (stack->size == 0){
//         STACK_ERROR(STACK_IS_ALREADY_EMPTY)
//     }
    
//     int top_elem = *stack->top;
//     *stack->top = 0;
//     if (stack->size != 1){
//         --stack->top;
//     }

//     --stack->size;

//     festtechreturn top_elem;
// }


int Pop(stack_t *stack){
    if (stack == nullptr){
        // DUMP
        return POISON;
    }
    VerifyStack(stack);

    if (stack->size == 0){
        STACK_ERROR(STACK_IS_ALREADY_EMPTY)
    }
    
    int top_elem = *--stack->top;
    *stack->top = 0xA0AA;
    stack->size--;

    VerifyStack(stack);

    return top_elem;
}



int Push(stack_t *stack, int elem){
    if (stack == nullptr){
        // DUMP
        return POISON;
    }

    VerifyStack(stack);
    // VERIFIED THAT STACK IS NORMAL

    if (stack->top == nullptr && stack->capacity == 0){
        stack->top = (int *) calloc(ADDITIONAL_SIZE, sizeof(int));
        if (stack->top == nullptr){
            STACK_ERROR(CANT_ALLOCATE_MEMORY)
        }
    
        stack->capacity += ADDITIONAL_SIZE;
    }
    
    if (stack->size + 1 == stack->capacity){
        int *root = stack->top - stack->size;
        printf("root at %p; val = %d\n", root, *root);
        
        root = (int *) realloc(root, sizeof(int) * (ADDITIONAL_SIZE + stack->capacity));
        
        stack->top = root + stack->size;
        stack->capacity += ADDITIONAL_SIZE;
    }

    *stack->top++ = elem;
    stack->size++;

    VerifyStack(stack);

}

int DestructStack(stack_t *stack){
    if (stack == nullptr){
        // LOG
        // DUMP
        return POISON;
    }
    VerifyStack(stack);

    if (stack->top == nullptr){
        if (stack->capacity == 0){
            stack->capacity = 0xBEBA; 
            stack->size     = 0xDEDA;
            return 0;
        }
        STACK_ERROR(STACK_TOP_IS_NULLPTR)
    }


    while (stack->size > 0){
        *--stack->top = stack->size-- * 0x707A;
    }

    free(stack->top);
    stack->top = nullptr;

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
        case_of_switch(STACK_IS_OVERFLOWED)
        case_of_switch(STACK_TOP_IS_NULLPTR)
        
        case_of_switch(CANT_ALLOCATE_MEMORY)

        default: return "UNDEFINED_ERROR";
    }
}




int VerifyStack(stack_t *stack){ // TODO && USE THIS SOMEHOW

    if (stack->error){
        // todo
    }
    // DUMP

    if (stack == nullptr){
        
        return STACK_IS_NULLPTR;
    }
    
    if (stack->size > stack->capacity){
        // some error
    }

    if (stack->top == nullptr && stack->capacity != 0){
        // top is dead
    }

    stack->error = 0;
    
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////

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

