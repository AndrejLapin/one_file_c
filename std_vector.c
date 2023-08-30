#include <stdio.h>

#define vector(type) \
typedef struct vector_##type { \
    type value; \
}vector_##type##_t; \
void print_vector_##type##_value (vector_##type##_t t) { \
    printf("Value is: %d\n", t.value); \
}\

vector(int);

int main()
{
    vector_int_t array;
    array.value = 5;
    print_vector_int_value(array);

    return 0;
}