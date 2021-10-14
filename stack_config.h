#ifndef STACK_CONFIG_H

#define STACK_CONFIG_H

#define DEBUG_MODE   07

#include <stdio.h>

typedef int Elem_t;

const Elem_t POISONED_ELEM = 3802;

void PrintDec(void *memory, size_t size, FILE *stream);

#endif