#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <time.h>

#include "../src/ptime.h"

int __wrap_printf(const char *format, int hour, int minute, int second) {
    check_expected(hour);
    check_expected(minute);
    check_expected(second);
    return 0;
}

time_t __wrap_time(void) {
    return (time_t) mock();
}

struct tm __wrap_localtime(time_t *t) {
    struct tm time;
    time.tm_hour = 1;
    time.tm_min = 1;
    time.tm_sec = 1;
    return time;
}

static void test_ptime_passes_correct_time_to_printf(void **state) {
    (void) state; /* unused */

    will_return(__wrap_time, 1590693361);
    expect_value(__wrap_printf, hour, 1);
    expect_value(__wrap_printf, minute, 1);
    expect_value(__wrap_printf, second, 1);

    print_time();
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ptime_passes_correct_time_to_printf),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}