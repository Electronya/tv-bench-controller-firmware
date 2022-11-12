#include <zephyr/ztest.h>

void dummy(void)
{
  zassert_true(true, "dummy test");
}

void test_main(void)
{
  ztest_test_suite(utils_suite,
    ztest_unit_test(dummy));
  ztest_run_test_suite(utils_suite);
}
