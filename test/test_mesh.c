#include "unity.h"
#include "../src/mesh.h"



void setUp(void) {

}

void tearDown(void) {
}

void test_load_obj(void) {
    mesh_t mesh = mesh_create();
    mesh_load_file(mesh, "./assets/cube.obj");
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_load_obj);
    return UNITY_END();
}
