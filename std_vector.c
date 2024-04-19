#include <stdio.h>

// but it dont know the type
// maybe it would work better in c++ with templating?
// but we only need generic types
// this bs dont make sense

// if input was something like "We typing {variable}"
// after { we have to start doing magic,
// after { start variable name and variable name ends at }
// this deffinetly looks like a preprocessor job
// but what is really possible??
#define string_format()

#define vector(type) \
typedef struct vector_##type##_t { \
    type value; \
}vector_##type##_t; \
void print_vector_##type##_value (vector_##type##_t* t) { \
    printf("Value is: %d\n", t->value); \
}

#define vector_ptr_type(type) \
typedef type* ##type##_ptr;

#define vector_ptr(type) \
typedef type* ptr_##type; \
vector(ptr_##type)

vector(int);
vector_ptr_type(int);
vector(int_ptr);
//vector_ptr(int);

int main()
{
    vector_int_t array;
    array.value = 5;
    print_vector_int_value(&array);

    //ptr_int intPointer;
    //vector_ptr_int_t tooLong;

    //vectorp_int_t something;

    return 0;
}