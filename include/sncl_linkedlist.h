/* SNCL ArrayList v1.00
   Defines an interface for dynamic custom-type linkedlists in C.

   Contributors:
   - StarIitNova (fynotix.dev@gmail.com)
 */

#ifndef SNCL_LINKEDLIST_H__
#define SNCL_LINKEDLIST_H__

#include <stdbool.h>
#include <stddef.h>

#include "sncl_typeid.h"

#define linked_list(type) type *

//// construction

// Creates a linked list given the type's size.
// It is recommended that you use `linked_list_new` instead which allows you to pass the type directly.
void *linked_list__create(size_t type_size);
// Destroys a linked list given its pointer.
// It is recommended that you use `linked_list_destroy` instead.
void linked_list__destroy(void *ll);

//// iterators

// Starts the internal iterator of the linkedlist at the first element.
// It is recommended that you use `linked_list_start` instead.
void linked_list__start(void *ll);
// Starts the internal iterator of the linkedlist at the last element.
// It is recommended that you use `linked_list_rstart` instead.
void linked_list__rstart(void *ll);
// Shifts the internal iterator of the linkedlist once, returning the value once stored by the iterator.
// It is recommended that you use `linked_list_next` instead which returns a copy of the value.
void *linked_list__next(void *ll);
// Removes the element pointed to by the internal iterator of the linked list.
void linked_list_it_remove(void *ll);

// Shifts the passed iterator up once, returning it afterwards
iterator_t linked_list_it_next(iterator_t current);

// Returns the starting iterator of the linkedlist.
iterator_t linked_list_begin(void *ll);
// Returns `NULL`, an indication that the last element has been reached.
iterator_t linked_list_end(void *ll);

//// value stuff

// Returns the element at the front of the linked list.
// It is recommended that you use `linked_list_front` instead which returns a copy of the element.
void *linked_list__front(void *ll);
// Returns the element at the back of the linked list.
// It is recommended that you use `linked_list_back` instead which returns a copy of the element.
void *linked_list__back(void *ll);
// Returns the element at the given index of the linked list.
// This function runs in `O(n)` complexity, please use an arraylist instead if you need fast random access.
// It is recommended that you use `linked_list_at` instead which returns a copy of the element.
void *linked_list__get(void *ll, size_t offset);

// Returns whether the linkedlist's size is 0 or not.
bool linked_list_empty(void *ll);
// Returns the size of the linkedlist
size_t linked_list_size(void *ll);

// Clears the linkedlist, freeing all nodes and setting the size to 0.
// This fuction runs in `O(n)` complexity, please use an arraylist instead if you need fast cleanup.
void linked_list_clear(void *ll);

// Adds an element to the linkedlist, supporting only lvalues (sorry).
// This function runs in `O(n)` complexity.
// It is recommended that you use `linked_list_add` instead which will handle converting your value to a pointer for
// you.
void linked_list__add(void *ll, size_t pos, void *val);
// Pushes an element to the front of the linkedlist, supporting only lvalues (sorry).
// It is recommended that you use `linked_list_push_front` instead which will handle converting your value to a pointer
// for you.
void linked_list__push_front(void *ll, void *val);
// Pushes an element to the back of the linkedlist, supporting only lvalues (sorry).
// It is recommended that you use `linked_list_push_back` instead which will handle converting your value to a pointer
// for you.
void linked_list__push_back(void *ll, void *val);

// Removes an element from the linkedlist given it's position.
// This function runs in `O(2n)` complexity, it is recommended you opt to use the internal iterators for removal.
// It is recommended that you use `linked_list_remove` instead which returns a copy of the value.
void linked_list__remove(void *ll, size_t offset);
// Removes the first element from the linkedlist and returns it.
// It is recommended that you use `linked_list_pop_front` instead which returns a copy of the value.
void linked_list__pop_front(void *ll);
// Removes the last element from the linkedlist and returns it.
// It is recommended that you use `linked_list_pop_back` instead which returns a copy of the value.
void linked_list__pop_back(void *ll);

//// macros

#define linked_list_new(type) ((type *)linked_list__create(sizeof(type)))
#define linked_list_destroy(ll) linked_list__destroy((void *)(ll))

#define linked_list_start(ll) linked_list__start((void *)(ll));
#define linked_list_rstart(ll) linked_list__rstart((void *)(ll));
#define linked_list_next(ll) (*((typeof(ll))(linked_list__next((void *)(ll)))))

#define linked_list_front(ll) (*((typeof(ll))(linked_list__front((void *)(ll)))))
#define linked_list_back(ll) (*((typeof(ll))(linked_list__back((void *)(ll)))))
#define linked_list_at(ll, idx) (*((typeof(ll))(linked_list__get((void *)(ll), idx))))

#define linked_list_add(ll, idx, val) linked_list__add((void *)(ll), idx, &(val))
#define linked_list_push_front(ll, val) linked_list__push_front((void *)(ll), &(val))
#define linked_list_push_back(ll, val) linked_list__push_back((void *)(ll), &(val))

#define linked_list_remove(ll, idx)                                                                                    \
    linked_list_at(ll, idx);                                                                                           \
    linked_list__remove((void *)(ll), idx)
#define linked_list_pop_front(ll)                                                                                      \
    linked_list_front(ll);                                                                                             \
    linked_list__pop_front(ll)
#define linked_list_pop_back(ll)                                                                                       \
    linked_list_back(ll);                                                                                              \
    linked_list__pop_back(ll)

#endif // SNCL_LINKEDLIST_H__
