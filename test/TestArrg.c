#include "../unity/src/unity.h"
#include "../src/Arrg.c"


void test_find_max_lform_size_returns_longest_lform_length(void) {
    ar_conf cfgv[] = {
        {'a', "dry-run", "description here", 0},
        {'b', "include-lib", "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv); 
    int a = find_max_lform_size(parser);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(11, a);
}

void test_find_max_lform_size_returns_zero_1(void) {
    ar_conf cfgv[] = {
        {'b', "", "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv); 
    int a = find_max_lform_size(parser);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_find_max_lform_size_returns_zero_2(void) {
    ar_conf cfgv[] = {
        {'b', NULL, "description here", 0},
        {'c', NULL, "description", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv); 
    int a = find_max_lform_size(parser);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_find_max_lform_size_returns_zero_3(void) {
    ar_parser *parser = ar_init(0, NULL, 0, NULL); 
    int a = find_max_lform_size(parser);
    ar_close(parser);
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
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'b');
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(0, a);
}

void test_get_sform_index_returns_index_2(void) {
    ar_conf cfgv[] = { 
        {'\0', NULL, "FILE", 0},
        {'b', "blank", "show whitespace", 0} 
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'b');
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(1, a);
}

void test_get_sform_index_returns_index_3(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'd');
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(2, a);
}

void test_get_sform_index_returns_notFound_1(void) {
    ar_conf cfgv[] = {};
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'a');
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_sform_index_returns_notFound_2(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'a');
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_sform_index_returns_notFound_3(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_sform_index(parser, 'a');
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_lform_index_returns_index_1(void) {
    ar_conf cfgv[] = { 
        {'b', NULL, "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_lform_index(parser, "--copy");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(1, a);
}

void test_get_lform_index_returns_notFound_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_lform_index(parser, NULL);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_get_lform_index_returns_notFound_2(void) {
    ar_conf cfgv[] = {};
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    int a = get_lform_index(parser, NULL);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(-1, a);
}

void test_add_value_returns_success_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int a = add_value(parser, 1, "haha");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
}

void test_add_value_returns_success_2(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0 | AR_MULTI_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    add_value(parser, 1, "haha");
    int a = add_value(parser, 1, "hoho");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
}

void test_add_value_returns_tm_vals_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'c', "copy", "", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    add_value(parser, 1, "haha");
    int a = add_value(parser, 1, "hoho");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(TM_VALS, a);
}

void test_add_value_returns_tm_pos_args_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'\0', NULL, "FILE", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    add_value(parser, 1, "haha");
    int a = add_value(parser, 1, "hoho");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(TM_POS_ARGS, a);
}


void test_add_positional_returns_success_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'\0', NULL, "FILE", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int a = add_positional(parser, "hoho");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
}

void test_add_positional_returns_no_pos_args_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int a = add_positional(parser, "hoho");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(NO_POS_ARGS, a);
}

void test_add_positional_returns_tm_pos_args_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", 0},
        {'\0', NULL, "FILE", 0 | AR_ONE_VAL},
        {'d', "delete", NULL, 0}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    ar_parser *parser = ar_init(0, NULL, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int a = add_positional(parser, "hoho");
    a = add_positional(parser, "haha");
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(TM_POS_ARGS, a);
}

void test_handle_sform_returns_success_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_NO_VAL}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-b"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-b", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(1, arg_idx);
}

void test_handle_sform_returns_success_2(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL | AR_MANDATORY}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-d", "file.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-d", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(2, arg_idx); //has been incremented because value is the next argument
}

void test_handle_sform_returns_success_3(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL | AR_MANDATORY}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-bd", "file.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-bd", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(2, arg_idx); //has been incremented because value is the next argument
}

void test_handle_sform_returns_success_4(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL | AR_MANDATORY}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-dfile.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-dfile.txt", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(1, arg_idx); //has been incremented because value is the next argument
}

void test_handle_sform_returns_success_5(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL | AR_MANDATORY}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-bdfile.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-bdfile.txt", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(1, arg_idx); //has been incremented because value is the next argument
}

void test_handle_sform_returns_inv_opt_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL | AR_MANDATORY}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-f", "file.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-f", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(INV_OPT, a);
}

void test_handle_sform_returns_no_val_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL | AR_MANDATORY}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "-d"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_sform(parser, "-d", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(NO_VAL, a);
}

void test_handle_lform_returns_success_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_NO_VAL}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "--blank"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_lform(parser, "--blank", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(1, arg_idx);
}

void test_handle_lform_returns_success_2(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "--delete", "file.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_lform(parser, "--delete", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(2, arg_idx);
}

void test_handle_lform_returns_success_3(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_ONE_VAL}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "--delete=file.txt"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_lform(parser, "--delete=file.txt", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(SUCCESS, a);
    TEST_ASSERT_EQUAL_INT(1, arg_idx);
}

void test_handle_lform_returns_inv_opt_1(void) {
    ar_conf cfgv[] = { 
        {'b', "blank", "show whitespace", AR_NO_VAL},
        {'\0', NULL, "FILE", AR_ONE_VAL},
        {'d', "delete", NULL, AR_NO_VAL}
    };
    int cfgc = sizeof(cfgv) / sizeof(ar_conf);
    char *argv[] = {"utilname", "--blanks"};
    int argc = sizeof(argv) / sizeof(char*);
    ar_parser *parser = ar_init(argc, argv, cfgc, cfgv);
    ar_exit_on_error(parser, false);
    int arg_idx = 1;
    int a = handle_lform(parser, "--blanks", &arg_idx);
    ar_close(parser);
    TEST_ASSERT_EQUAL_INT(INV_OPT, a);
    TEST_ASSERT_EQUAL_INT(1, arg_idx);
}

void setUp(void) {}
void tearDown(void) {}

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
    RUN_TEST(test_add_value_returns_success_2);
    RUN_TEST(test_add_value_returns_tm_vals_1);
    RUN_TEST(test_add_value_returns_tm_pos_args_1);
    RUN_TEST(test_add_positional_returns_success_1);
    RUN_TEST(test_add_positional_returns_no_pos_args_1);
    RUN_TEST(test_add_positional_returns_tm_pos_args_1);
    RUN_TEST(test_handle_sform_returns_success_1);
    RUN_TEST(test_handle_sform_returns_success_2);
    RUN_TEST(test_handle_sform_returns_success_3);
    RUN_TEST(test_handle_sform_returns_success_4);
    RUN_TEST(test_handle_sform_returns_success_5);
    RUN_TEST(test_handle_sform_returns_inv_opt_1);
    RUN_TEST(test_handle_sform_returns_no_val_1);
    RUN_TEST(test_handle_lform_returns_success_1);
    RUN_TEST(test_handle_lform_returns_success_2);
    RUN_TEST(test_handle_lform_returns_success_3);
    RUN_TEST(test_handle_lform_returns_inv_opt_1);
    return UNITY_END();
}
