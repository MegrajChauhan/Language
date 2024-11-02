#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec.h"

#define TEST_NORMAL_VEC_LEN 10
#define TEST_NORMAL_VEC_ELEM sizeof(int)
#define TEST_LARGE_VEC_ELEM sizeof(long)
#define TEST_STRING_VEC_ELEM sizeof(char *)
#define TEST_PERSON_VEC_ELEM sizeof(Person)

// Helper functions to print and clean up elements
void normal_vector_dump(void *elem)
{
    printf("ELEMENT: %d\n", *(int *)elem);
}

void large_vector_dump(void *elem)
{
    printf("ELEMENT: %ld\n", *(long *)elem);
}

void string_vector_dump(void *elem)
{
    printf("STRING ELEMENT: %s\n", *(char **)elem);
}

void string_vector_cleanup(void *elem)
{
    free(*(char **)elem);
}

// Struct for testing vector with structs
typedef struct
{
    char name[50];
    int age;
    int id;
} Person;

void person_vector_dump(void *elem)
{
    Person *p = (Person *)elem;
    printf("PERSON: Name=%s, Age=%d, ID=%d\n", p->name, p->age, p->id);
}

void person_vector_cleanup(void *elem)
{
    // If any fields need freeing (e.g., dynamically allocated), handle here
    // For now, we don't need any cleanup since `name` is not dynamically allocated.
}

// Normal integer vector test
void normal_vector_test()
{
    printf("NORMAL VECTOR TEST\n");
    vec *v = vec_init(TEST_NORMAL_VEC_ELEM, TEST_NORMAL_VEC_LEN);
    if (!v)
        crash();

    size_t i = 0;
    printf("POPULATING VECTOR\n");
    while (!vec_full(v))
    {
        if (!vec_push(v, &i))
        {
            vec_destroy(v);
            crash();
        }
        i++;
    }

    printf("VECTOR CONTENT\n");
    vec_dump(v, normal_vector_dump);

    printf("PUSHING BEYOND CAPACITY\n");
    for (; i < 50; i++)
    {
        if (!vec_push(v, &i))
        {
            vec_destroy(v);
            crash();
        }
    }

    printf("RESIZING VECTOR\n");
    if (!vec_resize(v, 2))
    {
        vec_destroy(v);
        crash();
    }

    printf("CAP OF VECTOR: %lu\n", v->elem_cap);
    vec_crunch(v);
    printf("AFTER CRUNCHING: %lu\n", v->elem_cap);

    int *_at = (int *)vec_at(v, 10);
    if (!_at)
    {
        vec_destroy(v);
        crash();
    }
    printf("AT INDEX 10: %d\n", *_at);

    printf("INDEX OF: %lu\n", vec_index_of(v, _at));

    while (!vec_empty(v))
    {
        vec_pop(v);
    }
    vec_destroy(v);
}

// Large integer vector test to push memory allocation limits
void large_vector_test()
{
    printf("LARGE VECTOR TEST\n");
    vec *v = vec_init(TEST_LARGE_VEC_ELEM, TEST_NORMAL_VEC_LEN);
    if (!v)
        crash();

    long val = 1000000;
    printf("POPULATING LARGE VECTOR\n");
    while (!vec_full(v))
    {
        if (!vec_push(v, &val))
        {
            vec_destroy(v);
            crash();
        }
        val += 1000000;
    }

    printf("LARGE VECTOR CONTENT\n");
    vec_dump(v, large_vector_dump);

    printf("RESIZING LARGE VECTOR\n");
    if (!vec_resize(v, 4))
    {
        vec_destroy(v);
        crash();
    }

    printf("CAP OF LARGE VECTOR: %lu\n", v->elem_cap);
    vec_crunch(v);
    printf("AFTER CRUNCHING: %lu\n", v->elem_cap);

    long *_at = (long *)vec_at(v, 5);
    if (!_at)
    {
        vec_destroy(v);
        crash();
    }
    printf("AT INDEX 5: %ld\n", *_at);

    while (!vec_empty(v))
    {
        vec_pop(v);
    }
    vec_destroy(v);
}

// String vector test with cleanup function
void string_vector_test()
{
    printf("STRING VECTOR TEST\n");
    vec *v = vec_init(TEST_STRING_VEC_ELEM, TEST_NORMAL_VEC_LEN);
    if (!v)
        crash();

    const char *sample_strings[] = {"apple", "banana", "cherry", "date", "elderberry"};
    printf("POPULATING STRING VECTOR\n");
    for (size_t i = 0; i < 5; i++)
    {
        char *str = strdup(sample_strings[i]);
        if (!vec_push(v, &str))
        {
            vec_destroy(v);
            crash();
        }
    }

    printf("STRING VECTOR CONTENT\n");
    vec_dump(v, string_vector_dump);

    printf("CLEANING UP STRING VECTOR\n");
    vec_clean(v, string_vector_cleanup);
    vec_destroy(v);
}

// Struct vector test
void person_vector_test()
{
    printf("PERSON VECTOR TEST\n");
    vec *v = vec_init(TEST_PERSON_VEC_ELEM, TEST_NORMAL_VEC_LEN);
    if (!v)
        crash();

    Person people[] = {
        {"Alice", 30, 1},
        {"Bob", 25, 2},
        {"Charlie", 35, 3},
        {"Diana", 28, 4},
        {"Eve", 40, 5}};

    printf("POPULATING PERSON VECTOR\n");
    for (size_t i = 0; i < 5; i++)
    {
        if (!vec_push(v, &people[i]))
        {
            vec_destroy(v);
            crash();
        }
    }

    printf("PERSON VECTOR CONTENT\n");
    vec_dump(v, person_vector_dump);

    printf("CLEANING UP PERSON VECTOR\n");
    vec_clean(v, person_vector_cleanup);
    vec_destroy(v);
}

// Stress test with high volume of elements
void stress_test()
{
    printf("STRESS TEST\n");
    vec *v = vec_init(TEST_NORMAL_VEC_ELEM, 1); // Start with minimal capacity
    if (!v)
        crash();

    printf("POPULATING STRESS TEST VECTOR\n");
    for (size_t i = 0; i < 10000; i++)
    { // Push a large number of elements
        if (!vec_push(v, &i))
        {
            vec_destroy(v);
            crash();
        }
    }

    printf("STRESS TEST VECTOR CONTENT (first 10 elements)\n");
    for (size_t i = 0; i < 10; i++)
    {
        int *elem = (int *)vec_at(v, i);
        printf("ELEMENT %lu: %d\n", i, *elem);
    }

    printf("POP ALL ELEMENTS\n");
    while (!vec_empty(v))
    {
        vec_pop(v);
    }
    vec_destroy(v);
}

void error_test()
{
    vec *v = vec_init(1, 1);
    if (!v)
        crash();
    // vec_at(v, 1);
    vec_destroy(v);
    v = NULL;
    // vec_destroy(v); // invalid
}

int main()
{
    normal_vector_test();
    large_vector_test();
    string_vector_test();
    person_vector_test();
    stress_test();
    error_test();
    return 0;
}
