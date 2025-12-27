#include "../unity/src/unity.h"
#include "../src/Arrg.c"


void test_find_max_lform_size_returns_longest_lform_length(void) {
    Ar_conf cfgv[] = {
        {'a', "dry-run", "description here", 0},
        {'b', "include-lib", "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv); 
    int a = find_max_lform_size(parser);
    TEST_ASSERT_EQUAL_INT(11, a);
}

void test_find_max_lform_size_returns_zero_1(void) {
    Ar_conf cfgv[] = {
        {'b', "", "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv); 
    int a = find_max_lform_size(parser);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_find_max_lform_size_returns_zero_2(void) {
    Ar_conf cfgv[] = {
        {'b', NULL, "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv); 
    int a = find_max_lform_size(parser);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_find_max_lform_size_returns_zero_3(void) {
    ar_parser *parser = ar_init(0, NULL, 0, NULL); 
    int a = find_max_lform_size(parser);
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

void test_get_sform_index_returns_index_1(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'b');
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_get_sform_index_returns_index_2(void) {
    Ar_conf cfgv[] = { 
        {'\0', NULL, "FILE", 0},
        {'b', "blank", "show whitespace", 0} 
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'b');
    TEST_ASSERT_EQUAL_INT(1, a);
}

void test_get_sform_index_returns_index_3(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'd');
    TEST_ASSERT_EQUAL_INT(2, a);
}

void test_get_sform_index_returns_notFound_1(void) {
    Ar_conf cfgv[] = {};
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'a');
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_sform_index_returns_notFound_2(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'a');
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_sform_index_returns_notFound_3(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'a');
    TEST_ASSERT_EQUAL_INT(-1, a);
}


void test_get_lform_index_returns_index_1(void) {
    Ar_conf cfgv[] = { 
        {'b', NULL, "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_lform_index(parser, "--copy");
    TEST_ASSERT_EQUAL_INT(1, a);
}

void test_get_lform_index_returns_notFound_1(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_lform_index(parser, NULL);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_lform_index_returns_notFound_2(void) {
    Ar_conf cfgv[] = {};
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_lform_index(parser, NULL);
    TEST_ASSERT_EQUAL_INT(-1, a);
}


void test_add_value_returns_success_1(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int a = add_value(parser, 1, "haha");
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
}

void test_add_value_returns_tm_args_1(void) {
    Ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(Ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    add_value(parser, 1, "haha");
    int a = add_value(parser, 1, "hoho");
    TEST_ASSERT_EQUAL_INT(TM_ARGS, a);
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
    RUN_TEST(test_get_sform_index_returns_index_1);
    RUN_TEST(test_get_sform_index_returns_index_2);
    RUN_TEST(test_get_sform_index_returns_index_3);
    RUN_TEST(test_get_sform_index_returns_notFound_1);
    RUN_TEST(test_get_sform_index_returns_notFound_2);
    RUN_TEST(test_get_sform_index_returns_notFound_3);
    RUN_TEST(test_get_lform_index_returns_index_1);
    RUN_TEST(test_get_lform_index_returns_notFound_1);
    RUN_TEST(test_get_lform_index_returns_notFound_2);
    RUN_TEST(test_add_value_returns_success_1);
    RUN_TEST(test_add_value_returns_tm_args_1);
    return UNITY_END();
}
