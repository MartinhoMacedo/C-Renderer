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

void test_matrix_multiplication(void) {
    float a_data[4][4] = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      { 13, 14, 15, 16 }};

    float b_data[4][4] = {
      {16, 15, 14, 13},
      {12, 11, 10, 9},
      {8, 7, 6, 5},
      {4,3,2,1}};

    mat4_t a = mat4_create(a_data);
    mat4_t b = mat4_create(b_data);

    mat4_mul_mat4(a,b,a);

    #ifdef PRINTS
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(a, j, i);
            printf("%f ", el);
        }
        printf("\n");
    }
    #endif

    TEST_ASSERT_EQUAL(mat4_get_element(a, 0, 0), 80);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 0, 1), 70);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 0, 2), 60);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 0, 3), 50);

    TEST_ASSERT_EQUAL(mat4_get_element(a, 1, 0), 240);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 1, 1), 214);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 1, 2), 188);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 1, 3), 162);

    TEST_ASSERT_EQUAL(mat4_get_element(a, 2, 0), 400);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 2, 1), 358);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 2, 2), 316);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 2, 3), 274);

    TEST_ASSERT_EQUAL(mat4_get_element(a, 3, 0), 560);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 3, 1), 502);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 3, 2), 444);
    TEST_ASSERT_EQUAL(mat4_get_element(a, 3, 3), 386);

}

void test_world_matrix(void) {
    //vec4_t v = vec4_create(sqrtf(2)/2, sqrtf(2)/2, sqrtf(2)/2, 1);
    vec4_t v = vec4_create(0, 1, 0, 1);
    mat4_t scale = mat4_create_scale(2, 2, 1);
    mat4_t rotation_x = mat4_create_rotation_x(M_PI/2);
    mat4_t rotation_y = mat4_create_rotation_y(M_PI/2);
    mat4_t rotation_z = mat4_create_rotation_z(M_PI/4);
    mat4_t translation = mat4_create_translation(0, 0, 2);

    //mat4_t world = mat4_create(NULL);

    //mat4_make_world(scale, rotation_x, rotation_y, rotation_z, translation, world);

    mat4_t world = mat4_create_world(2, 2, 1,
                                     M_PI / 2, M_PI / 2, M_PI / 4,
                                     0, 0, 2);

    #ifdef PRINTS
    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));
    #endif

    vec4_mul_mat4(world, v, v);

    #ifdef PRINTS
    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));
    #endif

    TEST_ASSERT_EQUAL_FLOAT(vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v));
    TEST_ASSERT_EQUAL_FLOAT(2, vec3_get_z((vec3_t)v));

}

void test_projection_matrix(void) {
    vec4_t v = vec4_create(1, 1, -1, 1);

    printf("v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));

    mat4_t mat_projection = mat4_create_projection(M_PI/2, 3, 1);

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float el = mat4_get_element(mat_projection, j, i);
            printf("%f ", el);
        }
        printf("\n");
    }


    vec4_mul_mat4(mat_projection, v, v);

    printf("homogeneous v = (%f, %f, %f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v),
           vec3_get_z((vec3_t) v), vec4_get_w(v));


    vec4_homogeneous_to_cartesian(v, (vec3_t) v);

    printf("proj v = (%f, %f)\n", vec2_get_x((vec2_t)v), vec2_get_y((vec2_t)v));

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
    //RUN_TEST(test_rotation_matrix_z);
    RUN_TEST(test_matrix_multiplication);
    RUN_TEST(test_world_matrix);
    //RUN_TEST(test_projection_matrix);
    return UNITY_END();
}
