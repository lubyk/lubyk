#include "text_reporter.h"
#include "reporter.h"
#include "breadcrumb.h"
#include <stdlib.h>
#include <stdio.h>

static void text_reporter_start(TestReporter *reporter, const char *name);
static void text_reporter_finish(TestReporter *reporter, const char *name);
static void show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *name);
static void show_breadcrumb(const char *name, void *memo);

TestReporter *create_text_reporter() {
    TestReporter *reporter = create_reporter();
	reporter->start = &text_reporter_start;
	reporter->finish = &text_reporter_finish;
	reporter->show_fail = &show_fail;
	reporter->show_incomplete = &show_incomplete;
    return reporter;
}

static void text_reporter_start(TestReporter *reporter, const char *name) {
	reporter_start(reporter, name);
	if (get_breadcrumb_depth((Breadcrumb *)reporter->breadcrumb) == 1) {
		printf("\nRunning \"%s\"\n", get_current_from_breadcrumb((Breadcrumb *)reporter->breadcrumb));
	}
}

static void text_reporter_finish(TestReporter *reporter, const char *name) {
	reporter_finish(reporter, name);
	if (get_breadcrumb_depth((Breadcrumb *)reporter->breadcrumb) == 0) {
		printf(
		    "\nCompleted \"%s\" : %d test%s, %d assertion%s, %d failure%s, %d exception%s.\n",
				name,
				reporter->tests,
				reporter->tests == 1 ? "" : "s",
				reporter->assertions,
				reporter->assertions == 1 ? "" : "s",
				reporter->failures,
				reporter->failures == 1 ? "" : "s",
				reporter->exceptions,
				reporter->exceptions == 1 ? "" : "s");
	}
}

static void show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    int i = 0;
    printf("\n[ failure ] ");
    walk_breadcrumb(
            (Breadcrumb *)reporter->breadcrumb,
            &show_breadcrumb,
            (void *)&i);
    printf(" (file %s line %d)\n", file, line);
    if (message != NULL) {
        printf("            ");
        vprintf(message, arguments);
        printf("\n");
    }
        
}

static void show_incomplete(TestReporter *reporter, const char *name) {
    int i = 0;
    printf("\n[exception] ");
    walk_breadcrumb(
            (Breadcrumb *)reporter->breadcrumb,
            &show_breadcrumb,
            (void *)&i);
    printf(" (test \"%s\")\n            failed to complete\n", name);
}

static void show_breadcrumb(const char *name, void *memo) {
    if (*(int *)memo > 0) {
        printf("/%s", name);
    }
    (*(int *)memo)++;
}
