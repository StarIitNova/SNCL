#include <sncl_test.h>

#include <sncl_arraylist.h>

BEFORE_ALL() {}

BEFORE_EACH() {}

TEST_CASE(ArrayList_CreateEmpty) {
    array_list(int) list = array_list_new(int);

    ASSERT_TRUE(array_list_empty(list));
    ASSERT_EQUAL(array_list_size(list), 0);
    ASSERT_TRUE(array_list_capacity(list) >= 16);

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_PushBackSingle) {
    array_list(int) list = array_list_new(int);

    int v = 42;
    array_list_push_back(list, v);

    ASSERT_FALSE(array_list_empty(list));
    ASSERT_EQUAL(array_list_size(list), 1);
    ASSERT_EQUALFMT(array_list_front(list), 42, "%d != %d");
    ASSERT_EQUALFMT(array_list_back(list), 42, "%d != %d");

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_PushBackMultiple) {
    array_list(int) list = array_list_new(int);

    for (int i = 0; i < 10; i++) {
        array_list_push_back(list, i);
    }

    ASSERT_EQUAL(array_list_size(list), 10);
    ASSERT_EQUALFMT(array_list_front(list), 0, "%d != %d");
    ASSERT_EQUALFMT(array_list_back(list), 9, "%d != %d");

    for (int i = 0; i < 10; i++) {
        ASSERT_EQUAL(array_list_at(list, i), i);
    }

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_ResizeOnPush) {
    array_list(int) list = array_list_new(int);

    size_t initial_cap = array_list_capacity(list);

    for (int i = 0; i < (int)initial_cap + 5; i++) {
        array_list_push_back(list, i);
    }

    ASSERT_TRUE(array_list_capacity(list) > initial_cap);
    ASSERT_EQUAL(array_list_size(list), initial_cap + 5);

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_InsertRangeMiddle) {
    array_list(int) list = array_list_new(int);

    int base[] = { 1, 2, 5, 6 };
    for (int i = 0; i < 4; i++) {
        array_list_push_back(list, base[i]);
    }

    int insert_vals[] = { 3, 4 };
    array_list_insert(list, array_list_begin(list) + 2, insert_vals, insert_vals + 2);

    ASSERT_EQUALFMT(array_list_size(list), 6, "%zu != %d");

    for (int i = 0; i < 6; i++) {
        ASSERT_EQUALFMT(array_list_at(list, i), i + 1, "%d != %d");
    }

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_EraseRange) {
    array_list(int) list = array_list_new(int);

    for (int i = 0; i < 10; i++) {
        array_list_push_back(list, i);
    }

    array_list_erase(list, array_list_begin(list) + 3, array_list_begin(list) + 7);

    ASSERT_EQUAL(array_list_size(list), 6);

    int expected[] = { 0, 1, 2, 7, 8, 9 };
    for (int i = 0; i < 6; i++) {
        ASSERT_EQUAL(array_list_at(list, i), expected[i]);
    }

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_PopBack) {
    array_list(int) list = array_list_new(int);

    for (int i = 0; i < 5; i++) {
        array_list_push_back(list, i);
    }

    int popped = array_list_pop_back(list);

    ASSERT_EQUALFMT(popped, 4, "%d != %d");
    ASSERT_EQUAL(array_list_size(list), 4);
    ASSERT_EQUALFMT(array_list_back(list), 3, "%d != %d");

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_Clear) {
    array_list(int) list = array_list_new(int);

    for (int i = 0; i < 8; i++) {
        array_list_push_back(list, i);
    }

    size_t cap_before = array_list_capacity(list);

    array_list_clear(list);

    ASSERT_TRUE(array_list_empty(list));
    ASSERT_EQUAL(array_list_size(list), 0);
    ASSERT_EQUAL(array_list_capacity(list), cap_before);

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_Reserve) {
    array_list(int) list = array_list_new(int);

    array_list_reserve(list, 64);

    ASSERT_TRUE(array_list_capacity(list) >= 64);
    ASSERT_EQUAL(array_list_size(list), 0);

    array_list_destroy((void *)list);
    return 0;
}

TEST_CASE(ArrayList_ReservePreservesData) {
    array_list(int) list = array_list_new(int);

    for (int i = 0; i < 10; i++) {
        int val = i * 2;
        array_list_push_back(list, val);
    }

    array_list_reserve(list, 128);

    ASSERT_EQUAL(array_list_size(list), 10);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQUAL(array_list_at(list, i), i * 2);
    }

    array_list_destroy((void *)list);
    return 0;
}
