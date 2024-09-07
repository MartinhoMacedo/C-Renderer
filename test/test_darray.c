#include "unity.h"
#include "../src/darray.h"

#define ARRAY_SIZE(A) (sizeof(A)/sizeof((A)[0]))

void setUp(void) {

}

void tearDown(void) {
}

void test_darray(void) {
    darray_int_t darray = darray_int_create();
    int ints[] = {1,9,3,4,5,6,7};

    for (int i = 0; i < ARRAY_SIZE(ints); i++) {
        darray_int_push(darray, ints[i]);
    }

    for (int i = 0; i < ARRAY_SIZE(ints); i++) {
        int n = darray_int_get(darray, i);
        printf("%d\n", n);
    }
    printf("size: %d, occupied: %d \n", darray_int_get_capacity(darray) ,darray_int_get_occupied(darray));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_darray);
    return UNITY_END();
}
