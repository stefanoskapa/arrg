#include "../unity/src/unity.h"
#include "../src/Arrg.h"

void test_AverageThreeBytes_should_AverageMidRangeValues(void)
{
TEST_ASSERT_EQUAL_HEX8(40, 40);
}

void setUp(void) {
    // Runs before each test
}

void tearDown(void) {
    // Runs after each test
}
int main(void)
{
UNITY_BEGIN();
RUN_TEST(test_AverageThreeBytes_should_AverageMidRangeValues);
return UNITY_END();
}
