#include <sncl_arraylist.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t type_size;
    size_t capacity;
    size_t size;
    uint8_t data[];
} array_list_t;

array_list_t *retrieve_from_data(void *data_ptr) { return (array_list_t *)(data_ptr - offsetof(array_list_t, data)); }

void *array_list_create(size_t type_size, size_t init_cap) {
    array_list_t *list = (array_list_t *)malloc(sizeof(array_list_t) + type_size * init_cap);
    assert(list != NULL && "failed to allocate array list when instantiating");
    list->type_size = type_size;
    list->capacity = init_cap;
    list->size = 0;
    return (void *)list->data;
}

void array_list_destroy(void *list) {
    array_list_t *arr = retrieve_from_data(list);
    free(arr);
}

void *array_list_vbegin(void *list) { return list; }

void *array_list_vend(void *list) {
    array_list_t *arr = retrieve_from_data(list);
    return (void *)(arr->data + arr->size * arr->type_size);
}

bool array_list_empty(void *list) {
    array_list_t *arr = retrieve_from_data(list);
    return arr->size == 0;
}

size_t array_list_size(void *list) {
    array_list_t *arr = retrieve_from_data(list);
    return arr->size;
}

size_t array_list_capacity(void *list) {
    array_list_t *arr = retrieve_from_data(list);
    return arr->capacity;
}

void array_list_vpush_back(void **list, void *value) {
    array_list_t *arr = retrieve_from_data(*list);

    if (arr->size >= arr->capacity) {
        array_list_vreserve(list, arr->capacity * 2);
        arr = retrieve_from_data(*list);
    }

    memcpy(arr->data + arr->type_size * arr->size, value, arr->type_size);
    arr->size++;
}

void array_list_vinsert(void **list, void *at, void *begin, void *end) {
    array_list_t *arr = retrieve_from_data(*list);

    size_t added = ((uint8_t *)end - (uint8_t *)begin) / arr->type_size;

    size_t needed = arr->size + added;
    if (arr->capacity <= needed) {
        size_t expected = arr->capacity;
        while (expected < needed)
            expected *= 2;
        array_list_vreserve(list, expected);
        arr = retrieve_from_data(*list);
    }

    size_t offset = ((uint8_t *)at - arr->data) / arr->type_size;
    // move data up
    memmove((void *)((uint8_t *)at + added * arr->type_size), at, (arr->size - offset) * arr->type_size);
    // copy in inserted data
    memcpy(at, begin, added * arr->type_size);
    arr->size += added;
}

void array_list_clear(void *list) {
    array_list_t *arr = retrieve_from_data(list);
    arr->size = 0;
}

void array_list_vreserve(void **list, size_t cap) {
    array_list_t *arr = retrieve_from_data(*list);
    if (cap < arr->capacity)
        return;

    array_list_t *new_arr = realloc(arr, sizeof(array_list_t) + arr->type_size * cap);
    assert(new_arr != NULL && "failed to allocate new array when reserving capacity");
    new_arr->capacity = cap;
    *list = new_arr->data;
}

void array_list_erase(void *list, void *begin, void *end) {
    array_list_t *arr = retrieve_from_data(list);

    size_t offset = ((uint8_t *)end - arr->data) / arr->type_size;
    size_t noffset = ((uint8_t *)begin - arr->data) / arr->type_size;
    size_t removing = offset - noffset;

    memmove((void *)((uint8_t *)list + noffset * arr->type_size), (void *)((uint8_t *)list + offset * arr->type_size),
            (arr->size - offset) * arr->type_size);
    arr->size -= removing;
}
