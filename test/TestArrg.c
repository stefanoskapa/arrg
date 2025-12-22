#include "../unity/src/unity.h"
#include "../src/Arrg.c"


void test_find_max_lform_size_returns_longest_lform_length(void) {
    Ar_conf cfgv[] = {
        {'a', "dry-run", "description here", 0},
        {'b', "include-lib", "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    int a = find_max_lform_size(cfgc, cfgv);
    TEST_ASSERT_EQUAL_INT(11, a);
}

void test_find_max_lform_size_returns_zero_1(void) {
    Ar_conf cfgv[] = {
        {'b', "", "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    int a = find_max_lform_size(cfgc, cfgv);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_find_max_lform_size_returns_zero_2(void) {
    Ar_conf cfgv[] = {
        {'b', NULL, "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    int a = find_max_lform_size(cfgc, cfgv);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_find_max_lform_size_returns_zero_3(void) {
    int a = find_max_lform_size(0, NULL);
    TEST_ASSERT_EQUAL_INT(0, a);
}

//static bool lform_equals(char *arg, char *lform) {

void setUp(void) {
    // Runs before each test
}

void tearDown(void) {
    // Runs after each test
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_find_max_lform_size_returns_longest_lform_length);
    RUN_TEST(test_find_max_lform_size_returns_zero_1);
    RUN_TEST(test_find_max_lform_size_returns_zero_2);
    RUN_TEST(test_find_max_lform_size_returns_zero_3);
    return UNITY_END();
}
