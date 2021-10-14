#include <stdio.h>

void PrintDec(void *memory, size_t size, FILE *stream){
    fprintf(stream, "{%d}\n", *((int *)memory));
}