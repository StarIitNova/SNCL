#include <sncl_test.h>

#include <sncl_linkedlist.h>

TEST_CASE(LinkedList_CreateEmpty) {
    linked_list(int) list = linked_list_new(int);

    ASSERT_TRUE(linked_list_empty(list));
    ASSERT_EQUAL(linked_list_size(list), 0);

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_PushBackSingle) {
    linked_list(int) list = linked_list_new(int);

    int v = 42;
    linked_list_push_back(list, v);

    ASSERT_FALSE(linked_list_empty(list));
    ASSERT_EQUAL(linked_list_size(list), 1);
    ASSERT_EQUALFMT(linked_list_front(list), 42, "%d != %d");
    ASSERT_EQUALFMT(linked_list_back(list), 42, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_PushFrontSingle) {
    linked_list(int) list = linked_list_new(int);

    int v = 7;
    linked_list_push_front(list, v);

    ASSERT_EQUAL(linked_list_size(list), 1);
    ASSERT_EQUALFMT(linked_list_front(list), 7, "%d != %d");
    ASSERT_EQUALFMT(linked_list_back(list), 7, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_PushOrder) {
    linked_list(int) list = linked_list_new(int);

    int v1 = 1, v2 = 2, v3 = 0;

    linked_list_push_back(list, v1);
    linked_list_push_back(list, v2);
    linked_list_push_front(list, v3);

    ASSERT_EQUAL(linked_list_size(list), 3);
    ASSERT_EQUALFMT(linked_list_at(list, 0), v3, "%d != %d");
    ASSERT_EQUALFMT(linked_list_at(list, 1), v1, "%d != %d");
    ASSERT_EQUALFMT(linked_list_at(list, 2), v2, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_AddAtIndex) {
    linked_list(int) list = linked_list_new(int);

    int v1 = 1, v2 = 2, v3 = 3;
    linked_list_push_back(list, v1);
    linked_list_push_back(list, v3);
    linked_list_add(list, 1, v2);

    ASSERT_EQUAL(linked_list_size(list), 3);
    ASSERT_EQUALFMT(linked_list_at(list, 0), v1, "%d != %d");
    ASSERT_EQUALFMT(linked_list_at(list, 1), v2, "%d != %d");
    ASSERT_EQUALFMT(linked_list_at(list, 2), v3, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_RemoveAtIndex) {
    linked_list(int) list = linked_list_new(int);

    int v1 = 1, v2 = 2, v3 = 3;
    linked_list_push_back(list, v1);
    linked_list_push_back(list, v2);
    linked_list_push_back(list, v3);

    linked_list_remove(list, 1); // remove 2

    ASSERT_EQUAL(linked_list_size(list), 2);
    ASSERT_EQUALFMT(linked_list_at(list, 0), v1, "%d != %d");
    ASSERT_EQUALFMT(linked_list_at(list, 1), v3, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_PopFront) {
    linked_list(int) list = linked_list_new(int);

    int v1 = 10, v2 = 20;
    linked_list_push_back(list, v1);
    linked_list_push_back(list, v2);

    int v = linked_list_pop_front(list);
    ASSERT_EQUALFMT(v, v1, "%d != %d");
    ASSERT_EQUAL(linked_list_size(list), 1);
    ASSERT_EQUALFMT(linked_list_front(list), v2, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_PopBack) {
    linked_list(int) list = linked_list_new(int);

    int v1 = 10, v2 = 20;
    linked_list_push_back(list, v1);
    linked_list_push_back(list, v2);

    int v = linked_list_pop_back(list);
    ASSERT_EQUALFMT(v, v2, "%d != %d");
    ASSERT_EQUAL(linked_list_size(list), 1);
    ASSERT_EQUALFMT(linked_list_front(list), v1, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_IteratorTraversal) {
    linked_list(int) list = linked_list_new(int);

    for (int i = 0; i < 5; i++)
        linked_list_push_back(list, i);

    linked_list_start(list);

    for (int i = 0; i < 5; i++) {
        int v = linked_list_next(list);
        ASSERT_EQUALFMT(v, i, "%d != %d");
    }

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_IteratorRemove) {
    linked_list(int) list = linked_list_new(int);

    for (int i = 1; i < 5; ++i)
        linked_list_push_back(list, i);

    linked_list_start(list);

    while (!linked_list_empty(list)) {
        int v = linked_list_next(list);
        if (v % 2 == 0)
            linked_list_it_remove(list);
        if (linked_list_size(list) == 2)
            break;
    }

    ASSERT_EQUAL(linked_list_size(list), 2);
    ASSERT_EQUALFMT(linked_list_at(list, 0), 1, "%d != %d");
    ASSERT_EQUALFMT(linked_list_at(list, 1), 3, "%d != %d");

    linked_list_destroy(list);
    return 0;
}

TEST_CASE(LinkedList_Clear) {
    linked_list(int) list = linked_list_new(int);

    for (int i = 1; i < 3; ++i)
        linked_list_push_back(list, i);

    linked_list_clear(list);

    ASSERT_TRUE(linked_list_empty(list));
    ASSERT_EQUAL(linked_list_size(list), 0);

    linked_list_destroy(list);
    return 0;
}
