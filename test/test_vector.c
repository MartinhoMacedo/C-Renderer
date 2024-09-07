#include "unity.h"
#include "../src/vector.h"

vec2_t vec2;
vec3_t vec3;
vec4_t vec4;

void setUp(void) {
    vec2 = vec2_create(1, 2);
    vec3 = vec3_create(1, 2, 3);
    vec4 = vec4_create(1, 2, 3, 4);
}

void tearDown(void) {
}

void test_get_x(void) {
    float x2 = vec2_get_x(vec2);
    float x3 = vec2_get_x((vec2_t) vec3);
    float x4 = vec2_get_x((vec2_t) vec4);

    TEST_ASSERT_EQUAL(x2, 1);
    TEST_ASSERT_EQUAL(x3, 1);
    TEST_ASSERT_EQUAL(x4, 1);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_x);
    return UNITY_END();
}
