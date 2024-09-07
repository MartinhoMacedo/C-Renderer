#include "unity.h"
#include "../src/mesh.h"



void setUp(void) {

}

void tearDown(void) {
}

void test_load_obj(void) {
    mesh_load_file("./assets/cube.obj");
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_load_obj);
    return UNITY_END();
}
