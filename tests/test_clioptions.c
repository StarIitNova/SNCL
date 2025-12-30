#include <sncl_test.h>

#include <sncl_clioptions.h>

BEFORE_ALL() {}

BEFORE_EACH() { cliopt_end(); }

TEST_CASE(CLIOptions_ShortOption) {
    char *argv[] = { "prog", "-a", NULL };
    cliopt_register(1, 'a', NULL, false, "Option A", NULL);
    cliopt_start(2, argv);

    long opt = cliopt_get();
    ASSERT_EQUAL(opt, 1);
    ASSERT_EQUAL(cliopt_idx(), 1);
    ASSERT_EQUAL(cliopt_getarg(), NULL);

    long end = cliopt_get();
    ASSERT_EQUAL(end, CLI_OPT_END_OF_OPTS);

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_LongOption) {
    char *argv[] = { "prog", "--output", NULL };
    cliopt_register(2, '\0', "output", false, "Output file", NULL);
    cliopt_start(2, argv);

    long opt = cliopt_get();
    ASSERT_EQUAL(opt, 2);
    ASSERT_EQUAL(cliopt_idx(), 1);
    ASSERT_EQUAL(cliopt_getarg(), NULL);

    long end = cliopt_get();
    ASSERT_EQUAL(end, CLI_OPT_END_OF_OPTS);

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_ShortOptionWithArg) {
    char *argv[] = { "prog", "-I", "include", NULL };
    cliopt_register(3, 'I', NULL, true, "Include path", "DIR");
    cliopt_start(3, argv);

    long opt = cliopt_get();
    ASSERT_EQUAL(opt, 3);
    ASSERT_EQUALFMT(cliopt_idx(), 2, "%d != %d"); // 2 because -I implicitly skips one from the hint
    ASSERT_STREQUAL(cliopt_getarg(), "include");

    long end = cliopt_get();
    ASSERT_EQUAL(end, CLI_OPT_END_OF_OPTS);

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_LongOptionWithArg) {
    char *argv[] = { "prog", "--include", "inc", NULL };
    cliopt_register(4, '\0', "include", true, "Include path", "DIR");
    cliopt_start(3, argv);

    long opt = cliopt_get();
    ASSERT_EQUAL(opt, 4);
    ASSERT_STREQUAL(cliopt_getarg(), "inc");

    long end = cliopt_get();
    ASSERT_EQUAL(end, CLI_OPT_END_OF_OPTS);

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_UnknownOption) {
    char *argv[] = { "prog", "-z", NULL };
    cliopt_register(5, 'a', NULL, false, "Option A", NULL);
    cliopt_start(2, argv);

    long opt = cliopt_get();
    ASSERT_EQUAL(opt, CLI_OPT_UNKNOWN);
    ASSERT_STREQUAL(cliopt_getarg(), "-z");

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_FileArguments) {
    char *argv[] = { "prog", "file1.txt", "file2.txt", NULL };
    cliopt_start(3, argv);

    long opt1 = cliopt_get();
    ASSERT_EQUAL(opt1, CLI_OPT_FILE);
    ASSERT_STREQUAL(cliopt_getarg(), "file1.txt");

    long opt2 = cliopt_get();
    ASSERT_EQUAL(opt2, CLI_OPT_FILE);
    ASSERT_STREQUAL(cliopt_getarg(), "file2.txt");

    long end = cliopt_get();
    ASSERT_EQUAL(end, CLI_OPT_END_OF_OPTS);

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_EndOfArgs) {
    char *argv[] = { "prog", "--", "-a", NULL };
    cliopt_register(6, 'a', NULL, false, "Option A", NULL);
    cliopt_start(3, argv);

    long opt = cliopt_get();
    ASSERT_EQUAL(opt, CLI_OPT_END_OF_ARGS);

    opt = cliopt_get();
    ASSERT_EQUAL(opt, CLI_OPT_END_OF_ARGS); // it shouldn't be able to continue

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_MultipleOptions) {
    char *argv[] = { "prog", "-a", "--output", "out.txt", "file.txt", NULL };
    cliopt_register(1, 'a', NULL, false, "Option A", NULL);
    cliopt_register(2, '\0', "output", true, "Output file", "FILE");
    cliopt_start(5, argv);

    ASSERT_EQUAL(cliopt_get(), 1); // -a
    ASSERT_EQUAL(cliopt_get(), 2); // --output
    ASSERT_STREQUAL(cliopt_getarg(), "out.txt");
    ASSERT_EQUAL(cliopt_get(), CLI_OPT_FILE); // file.txt
    ASSERT_STREQUAL(cliopt_getarg(), "file.txt");
    ASSERT_EQUAL(cliopt_get(), CLI_OPT_END_OF_OPTS);

    cliopt_end();
    return 0;
}

TEST_CASE(CLIOptions_PrintHelp) {
    cliopt_register(1, 'a', "all", false, "Enable all features", NULL);
    cliopt_register(2, 'o', "output", true, "Write output to file", "FILE");

    STASH_STREAM(stdout);
    // We just call it to ensure it does not crash.
    cliopt_printhelp("Usage: prog [options]", "prog");

    RESTORE_STREAM(stdout);
    cliopt_end();
    return 0;
}
