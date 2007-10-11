#include "cgreen/cgreen.h"
#include <stdlib.h>
#include <stdio.h>

#include "memory_test.c"
#include "time_queue_test.c"

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    
    add_suite(suite, memory_test_suite()      );
    add_suite(suite, time_queue_test_suite()  );
    
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
