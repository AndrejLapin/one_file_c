#include <stdio.h>

#define vector(type) \
typedef struct vector_##type##_t { \
    type value; \
}vector_##type##_t; \
void print_vector_##type##_value (vector_##type##_t* t) { \
    printf("Value is: %d\n", t->value); \
}

#define vector_ptr(type) \
typedef type* ptr_##type; \
vector(ptr_##type)

vector(int);
vector_ptr(int);

int main()
{
    vector_int_t array;
    array.value = 5;
    print_vector_int_value(&array);

    ptr_int intPointer;
    vector_ptr_int_t tooLong;

    //vectorp_int_t something;

    return 0;
}