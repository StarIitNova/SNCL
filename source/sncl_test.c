#include <sncl_test.h>

#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(__MACH__) || defined(__unix__) || defined(unix)
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#define SIGNALS_USED
#endif

#if defined(__APPLE__) || defined(__MACH__)
#include <mach/mach_time.h>
#elif defined(__unix__) || defined(unix)
#include <time.h>
#else
#error "OS does not support timing tests"
#endif

test_case_t *all_tests = NULL;
test_void_t before_eachfn = NULL;
test_void_t before_allfn = NULL;

size_t num_tests = 0;

char *test_err_msg = NULL;
const char *last_test = NULL;

void gettime(double *out);

int execute_test(test_case_t *t);

#ifdef SIGNALS_USED
jmp_buf test_env;

char *get_signal_name(int sig);

void test_signal_handler(int sig) {
    fprintf(
        stderr,
        "    \x1b[1;31mFATAL\x1b[0m: Caught signal %d (%s) in %s, attempting to longjump back into test sequence.\n\n",
        sig, get_signal_name(sig), last_test ? last_test : "(no last test)");
    // exit(128 + sig);
    longjmp(test_env, sig);
}
#endif

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char **argv) {
    bool forking = true;
    if (argc > 1 && strcmp(argv[1], "--no-fork") == 0) {
        forking = false;
        printf("\x1b[33mWARN\x1b[0m: Process forking disabled; the first signal will crash the test.\n\n");
    }

#ifdef SIGNALS_USED
    if (forking) {
        signal(SIGABRT, test_signal_handler); // abort()
        signal(SIGFPE, test_signal_handler);  // divide by zero / math fault
        signal(SIGILL, test_signal_handler);  // illegal instruction
        signal(SIGTERM, test_signal_handler); // termination
        signal(SIGINT, test_signal_handler);  // Ctrl+C
        signal(SIGTRAP, test_signal_handler); // Trap
    }
#endif

    int failed = 0;

    if (before_allfn != NULL) {
        before_allfn();
    }

    for (test_case_t *t = all_tests; t != NULL; t = t->next) {
        last_test = t->name;

        if (before_eachfn != NULL) {
            before_eachfn();
        }

        double start, end;
        gettime(&start);

        int jmpval = setjmp(test_env);
        int success = 0;
        if (jmpval == 0) {
            if (forking)
                success = execute_test(t);
            else
                success = t->entry();
        } else {
            success = jmpval;
            printf("Signal caught\n");
            asprintf(&test_err_msg, "Process received signal %d (%s) while executing\n", jmpval,
                     get_signal_name(jmpval));
        }

        gettime(&end);

        if (success != 0) {
            printf("    \x1b[1;36m%s\x1b[0m \x1b[1;31mFAILED\x1b[0m with code \x1b[33m%i\x1b[0m in %fs:\n        %s\n",
                   t->name, success, end - start, test_err_msg);
            failed++;
        } else {
            printf("    \x1b[1;36m%s\x1b[0m \x1b[1;32mSUCCEEDED\x1b[0m in %fs\n", t->name, end - start);
        }

        if (test_err_msg != NULL) {
            free((void *)test_err_msg);
            test_err_msg = NULL;
        }

        printf("\n");
    }

    return failed;
}

void gettime(double *out) {
#if defined(_WIN32)
    static LARGE_INTEGER freq;
    static int initialized = 0;
    LARGE_INTEGER counter;

    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }
    QueryPerformanceCounter(&counter);
    *out = (double)counter.QuadPart / (double)freq.QuadPart;

#elif defined(__APPLE__)
    // macOS has mach_absolute_time(), which gives nanoseconds since boot
    static mach_timebase_info_data_t timebase;
    static int initialized = 0;
    uint64_t now = mach_absolute_time();

    if (!initialized) {
        mach_timebase_info(&timebase);
        initialized = 1;
    }
    double nanos = (double)now * timebase.numer / timebase.denom;
    *out = nanos / 1e9;

#elif defined(__unix__)
    // POSIX (Linux, BSD, etc.)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    *out = ts.tv_sec + ts.tv_nsec / 1e9;
#endif
}

int execute_test(test_case_t *t) {
#if defined(__APPLE__) || defined(__unix__)
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;

    pid_t pid = fork();
    if (pid == 0) {
        // child
        close(pipefd[0]); // close read end

        char *msg = NULL;
        int ret = t->entry(); // runs test, sets global test_err_msg if needed
        if (test_err_msg != NULL) {
            msg = test_err_msg;
        }
        if (msg) {
            size_t len = strlen(msg) + 1;
            write(pipefd[1], msg, len);
        }
        close(pipefd[1]);
        exit(ret);
    } else {
        close(pipefd[1]);
        char buf[1024] = { 0 };
        read(pipefd[0], buf, sizeof(buf) - 1);
        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0);

        test_err_msg = strdup(buf);

        if (WIFSIGNALED(status)) {
            asprintf(&test_err_msg, "Process received signal %d (%s) while executing", WTERMSIG(status),
                     get_signal_name(WTERMSIG(status)));
            return 128 + WTERMSIG(status);
        } else if (WIFEXITED(status)) {
            int ret = WEXITSTATUS(status);
            if (ret >= 128)
                return -256 + ret;
            return ret;
        }
        return -1; // unexpected
    }
#endif

    return 1;
}

#ifdef SIGNALS_USED
char *get_signal_name(int sig) {
    switch (sig) {
    case SIGSEGV:
        return "Segmentation fault";
    case SIGABRT:
        return "Signal abort";
    case SIGFPE:
        return "Floating point error (divide by zero)";
    case SIGILL:
        return "Illegal instruction (corrupt executable?)";
    case SIGBUS:
        return "Bus error";
    case SIGTERM:
        return "Termination requested";
    case SIGINT:
        return "Interrupted";
    case SIGTRAP:
        return "Trapped";
    default:
        return "Unknown signal";
    }
}
#endif
