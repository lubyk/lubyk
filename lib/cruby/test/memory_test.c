
void these_should_be_true() {
    assert_true (1);
    assert_false(0);
}


TestSuite *memory_test_suite() {
    TestSuite *suite = create_test_suite();
    add_test(suite, these_should_be_true);
    return suite;
}