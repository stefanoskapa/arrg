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

void test_lform_equals_returns_true(void) {
    bool a = lform_equals("--list", "list"); 
    TEST_ASSERT_TRUE(a);
}

void test_lform_equals_returns_false_1(void) {
    bool a = lform_equals(NULL, "list"); 
    TEST_ASSERT_FALSE(a);
}

void test_lform_equals_returns_false_2(void) {
    bool a = lform_equals("list", NULL); 
    TEST_ASSERT_FALSE(a);
}

void test_lform_equals_returns_false_3(void) {
    bool a = lform_equals(NULL, NULL); 
    TEST_ASSERT_FALSE(a);
}

void test_lform_equals_returns_false_4(void) {
    bool a = lform_equals("--l", "list"); 
    TEST_ASSERT_FALSE(a);
}

void test_lform_equals_returns_false_5(void) {
    bool a = lform_equals("--list", "li"); 
    TEST_ASSERT_FALSE(a);
}

void test_is_sform_returns_false_1(void) {
    bool a = is_sform("--s");
    TEST_ASSERT_FALSE(a);
}

void test_is_sform_returns_false_2(void) {
    bool a = is_sform(NULL);
    TEST_ASSERT_FALSE(a);
}


void test_is_sform_returns_false_3(void) {
    bool a = is_sform("-");
    TEST_ASSERT_FALSE(a);
}

void test_is_sform_returns_false_4(void) {
    bool a = is_sform("--");
    TEST_ASSERT_FALSE(a);
}

void test_is_sform_returns_true_1(void) {
    bool a = is_sform("-a");
    TEST_ASSERT_TRUE(a);
}

void test_is_sform_returns_true_2(void) {
    bool a = is_sform("-abc");
    TEST_ASSERT_TRUE(a);
}

void test_is_lform_returns_false_1(void) {
    bool a = is_lform("--");
    TEST_ASSERT_FALSE(a);
}

void test_is_lform_returns_false_2(void) {
    bool a = is_lform("-asdf");
    TEST_ASSERT_FALSE(a);
}

void test_is_lform_returns_false_3(void) {
    bool a = is_lform(NULL);
    TEST_ASSERT_FALSE(a);
}

void test_is_lform_returns_false_4(void) {
    bool a = is_lform("");
    TEST_ASSERT_FALSE(a);
}

void test_is_lform_returns_false_5(void) {
    bool a = is_lform("-f");
    TEST_ASSERT_FALSE(a);
}

void test_is_lform_returns_true_1(void) {
    bool a = is_lform("--f");
    TEST_ASSERT_TRUE(a);
}

void test_is_lform_returns_true_2(void) {
    bool a = is_lform("--include");
    TEST_ASSERT_TRUE(a);
}

void test_is_lform_returns_true_3(void) {
    bool a = is_lform("--dry-run");
    TEST_ASSERT_TRUE(a);
}

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
    RUN_TEST(test_lform_equals_returns_true);
    RUN_TEST(test_lform_equals_returns_false_1);
    RUN_TEST(test_lform_equals_returns_false_2);
    RUN_TEST(test_lform_equals_returns_false_3);
    RUN_TEST(test_lform_equals_returns_false_4);
    RUN_TEST(test_lform_equals_returns_false_5);
    RUN_TEST(test_is_sform_returns_false_1);
    RUN_TEST(test_is_sform_returns_false_2);
    RUN_TEST(test_is_sform_returns_false_3);
    RUN_TEST(test_is_sform_returns_false_4);
    RUN_TEST(test_is_sform_returns_true_1);
    RUN_TEST(test_is_sform_returns_true_2);
    RUN_TEST(test_is_lform_returns_false_1);
    RUN_TEST(test_is_lform_returns_false_2);
    RUN_TEST(test_is_lform_returns_false_3);
    RUN_TEST(test_is_lform_returns_false_4);
    RUN_TEST(test_is_lform_returns_false_5);
    RUN_TEST(test_is_lform_returns_true_1);
    RUN_TEST(test_is_lform_returns_true_2);
    RUN_TEST(test_is_lform_returns_true_3);
    return UNITY_END();
}
