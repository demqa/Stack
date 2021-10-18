#ifndef STACK_CONFIG_H

#define STACK_CONFIG_H

#include <stdio.h>

#define DEBUG_MODE 07

typedef int Elem_t;

const Elem_t POISONED_ELEM = 3802;

void PrintDec(void *memory, size_t size, FILE *stream);

#endif