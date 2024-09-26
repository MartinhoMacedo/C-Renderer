#include "unity.h"
#include "../src/matrix.h"
#include "../src/vector.h"
#include <stdio.h>
#include <math.h>


void setUp(void) {
  }

void tearDown(void) {

}

void test_null_matrix(void) {
    mat4_t m_null = mat4_create(NULL);
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m_null, j, i);
            //printf("%f\n", el);
            TEST_ASSERT_EQUAL(el, 0);
        }
    }
}

void test_identity_matrix(void) {
    mat4_t m_identity = mat4_create_identity();
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m_identity, j, i);
            //printf("%f ", el);
            if(i == j){TEST_ASSERT_EQUAL(el, 1);}
            else TEST_ASSERT_EQUAL(el, 0);
        }
        //printf("\n");
    }
}

void test_scale_matrix(void) {
    mat4_t m_scale = mat4_create_scale(4,3,2);
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m_scale, j, i);
            //printf("%f ", el);
            if(i == 0 && j == 0){TEST_ASSERT_EQUAL(el, 4);}
            else if(i == 1 && j == 1){TEST_ASSERT_EQUAL(el, 3);}
            else if(i == 2 && j == 2){TEST_ASSERT_EQUAL(el, 2);}
            else if(i == 3 && j == 3){TEST_ASSERT_EQUAL(el, 1);}
            else TEST_ASSERT_EQUAL(el, 0);
        }
        //printf("\n");
    }
}

void test_translation_matrix(void) {
    mat4_t m = mat4_create_translation(1, 2, 3);
    vec4_t v =  vec4_create(3, 2, 1, 1);

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

    // Print m
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m, j, i);
            printf("%f ", el);
        }
        printf("\n");
    }

    vec4_mul_mat4(m, v, v);

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

}

void test_rotation_matrix_x(void) {
    vec4_t v =  vec4_create(0, 1, 0, 1);

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

    // rotate vector 90 degrees (pi/2 rad)
    mat4_t m = mat4_create_rotation_x(M_PI/2);
    vec4_mul_mat4(m, v, v);


    // Print m
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m, j, i);
            printf("%f ", el);
        }
        printf("\n");
    }

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

}

void test_rotation_matrix_y(void) {
    vec4_t v = vec4_create(0, 0, 1, 1);

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

    // rotate vector 90 degrees (pi/2 rad)
    mat4_t m = mat4_create_rotation_y(M_PI/2);
    vec4_mul_mat4(m, v, v);

    // Print m
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m, j, i);
            printf("%f ", el);
        }
        printf("\n");
    }

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

}

void test_rotation_matrix_z(void) {
    vec4_t v =  vec4_create(1, 0, 0, 1);

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

    // rotate vector 90 degrees (pi/2 rad)
    mat4_t m = mat4_create_rotation_y(M_PI/2);
    vec4_mul_mat4(m, v, v);

    // Print m
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(m, j, i);
            printf("%f ", el);
        }
        printf("\n");
    }

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

}


void test_get_element(void) {
    float data[4][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1,1,1,1}};
    mat4_t m = mat4_create(data);

    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 4; i++) {
        float el = mat4_get_element(m, j, i);
        //printf("%f\n", el);
        TEST_ASSERT_EQUAL(el, 1);
      }
    }
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_element);
    RUN_TEST(test_identity_matrix);
    RUN_TEST(test_null_matrix);
    RUN_TEST(test_scale_matrix);
    //RUN_TEST(test_translation_matrix);
    //RUN_TEST(test_rotation_matrix_x);
    //RUN_TEST(test_rotation_matrix_y);
    ////RUN_TEST(test_rotation_matrix_z);
    return UNITY_END();
}
