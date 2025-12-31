#include <sncl_linkedlist.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct SNCL_LLNode sncl_llnode_t;

typedef struct {
    sncl_llnode_t *first;
    sncl_llnode_t *last;

    iterator_t it__;
    bool it_forward__;

    size_t size;
    size_t type_size;
} linkedlist_t;

struct SNCL_LLNode {
    sncl_llnode_t *next;
    sncl_llnode_t *prev;
    char data[];
};

#define node(v) ((sncl_llnode_t *)(v))
#define xor_ptr(a, b) (void *)((uintptr_t)(a) ^ (uintptr_t)(b))

static sncl_llnode_t *new_node(linkedlist_t *L, void *val);

linkedlist_t *retrieve_from_data(void *data_ptr) { return (linkedlist_t *)(data_ptr); }

void *linked_list__create(size_t type_size) {
    linkedlist_t *ll = (linkedlist_t *)malloc(sizeof(linkedlist_t));
    if (!ll)
        return NULL;

    ll->first = NULL;
    ll->last = NULL;
    ll->it__ = NULL;
    ll->size = 0;
    ll->type_size = type_size;

    return (void *)ll;
}

void linked_list__destroy(void *ll) {
    linked_list_clear(ll);
    free(ll);
}

void linked_list__start(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    L->it__ = (iterator_t)L->first;
    L->it_forward__ = true;
}

void linked_list__rstart(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    L->it__ = (iterator_t)L->last;
    L->it_forward__ = false;
}

void *linked_list__next(void *ll) {
    linkedlist_t *L = ll;
    if (!L->it__)
        return NULL;

    sncl_llnode_t *n = L->it__;
    if (L->it_forward__)
        L->it__ = n->next;
    else
        L->it__ = n->prev;

    return n->data;
}

void linked_list_it_remove(void *ll) {
    linkedlist_t *L = ll;

    sncl_llnode_t *to_remove;

    if (L->it__)
        to_remove = node(L->it__)->prev;
    else
        to_remove = L->last;

    if (!to_remove)
        return;

    if (to_remove->prev)
        to_remove->prev->next = to_remove->next;
    else
        L->first = to_remove->next;

    if (to_remove->next)
        to_remove->next->prev = to_remove->prev;
    else
        L->last = to_remove->prev;

    free(to_remove);
    L->size--;
}

iterator_t linked_list_it_next(iterator_t current) { return (iterator_t)(node(current)->next); }

iterator_t linked_list_begin(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    return (iterator_t)L->first;
}

iterator_t linked_list_end(__attribute__((unused)) void *ll) { return NULL; }

void *linked_list__front(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    return (void *)(L->first->data);
}

void *linked_list__back(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    return (void *)(L->last->data);
}

void *linked_list__get(void *ll, size_t offset) {
    linkedlist_t *L = retrieve_from_data(ll);
    if (offset == 0)
        return linked_list__front(ll);
    if (offset == L->size - 1)
        return linked_list__back(ll);
    if (offset >= L->size)
        return NULL;

    sncl_llnode_t *curr = L->first;
    while (offset > 0) {
        curr = curr->next;
        offset--;
    }

    return (void *)(curr->data);
}

bool linked_list_empty(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    return L->size == 0;
}

size_t linked_list_size(void *ll) {
    linkedlist_t *L = retrieve_from_data(ll);
    return L->size;
}

void linked_list_clear(void *ll) {
    linkedlist_t *L = ll;
    sncl_llnode_t *curr = L->first;

    while (curr) {
        sncl_llnode_t *next = curr->next;
        free(curr);
        curr = next;
    }

    L->first = L->last = NULL;
    L->size = 0;
}

void linked_list__add(void *ll, size_t pos, void *val) {
    linkedlist_t *L = ll;
    if (pos == 0)
        return linked_list__push_front(ll, val);
    if (pos == L->size)
        return linked_list__push_back(ll, val);
    if (pos > L->size)
        return;

    sncl_llnode_t *curr = L->first;
    for (size_t i = 0; i < pos; i++)
        curr = curr->next;

    sncl_llnode_t *n = new_node(L, val);
    n->prev = curr->prev;
    n->next = curr;

    curr->prev->next = n;
    curr->prev = n;

    L->size++;
}

void linked_list__push_front(void *ll, void *val) {
    linkedlist_t *L = ll;
    sncl_llnode_t *n = new_node(L, val);
    if (!n)
        return;

    n->next = L->first;
    if (L->first)
        L->first->prev = n;
    else
        L->last = n;

    L->first = n;
    L->size++;
}

void linked_list__push_back(void *ll, void *val) {
    linkedlist_t *L = ll;
    sncl_llnode_t *n = new_node(L, val);
    if (!n)
        return;

    n->prev = L->last;
    if (L->last)
        L->last->next = n;
    else
        L->first = n;

    L->last = n;
    L->size++;
}

void linked_list__remove(void *ll, size_t pos) {
    linkedlist_t *L = ll;
    if (pos >= L->size)
        return;
    if (pos == 0)
        return linked_list__pop_front(ll);
    if (pos == L->size - 1)
        return linked_list__pop_back(ll);

    sncl_llnode_t *curr = L->first;
    for (size_t i = 0; i < pos; i++)
        curr = curr->next;

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    free(curr);
    L->size--;
}

void linked_list__pop_front(void *ll) {
    linkedlist_t *L = ll;
    if (!L->first)
        return;

    sncl_llnode_t *n = L->first;
    L->first = n->next;

    if (L->first)
        L->first->prev = NULL;
    else
        L->last = NULL;

    free(n);
    L->size--;
}

void linked_list__pop_back(void *ll) {
    linkedlist_t *L = ll;
    if (!L->last)
        return;

    sncl_llnode_t *n = L->last;
    L->last = n->prev;

    if (L->last)
        L->last->next = NULL;
    else
        L->first = NULL;

    free(n);
    L->size--;
}

static sncl_llnode_t *new_node(linkedlist_t *L, void *val) {
    sncl_llnode_t *n = malloc(sizeof(sncl_llnode_t) + L->type_size);
    if (!n)
        return NULL;

    memcpy(n->data, val, L->type_size);
    n->next = NULL;
    n->prev = NULL;
    return n;
}
