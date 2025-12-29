/* SNCL ArrayList v1.0
   Defines an interface for dynamic custom-type arrays in C.

   Contributors:
   - StarIitNova (fynotix.dev@gmail.com)
 */

#ifndef SNCL_ARRAYLIST_H__
#define SNCL_ARRAYLIST_H__

#include <stdbool.h>
#include <stddef.h>

// Standard definition for a public arraylist type
#define array_list(type) type *

// Creates an arraylist given the type size and initial capacity, returning it as a `void*`.
// It is recommended that you use the public facing API `array_list_new(type)`.
void *array_list_create(size_t type_size, size_t init_cap);
// Destroys an arraylist. We recommend you cast it to `void *` beforehand, ex `array_list_destroy((void *)list);`.
void array_list_destroy(void *list);

// Returns the beginning pointer of the arraylist as `void *`.
void *array_list_vbegin(void *list);
// Returns the end pointer of the arraylist as `void *`.
void *array_list_vend(void *list);

// Returns `true` if the arraylist contains `0` elements, and `false` otherwise.
bool array_list_empty(void *list);
// Returns the size of the arraylist.
size_t array_list_size(void *list);
// Returns the capacity of the arraylist, as in the max size the arraylist can store before resizing.
size_t array_list_capacity(void *list);

// Voided pushback method. Pushes a value to the end of the arraylist.
// It is recommended that you use the public facing API `array_list_push_back(list, value)`.
void array_list_vpush_back(void **list, void *value);
// Voided insert method. Inserts a range of values at a specific index in the arraylist.
// It is recommended that you use the public facing API `array_list_insert(list, pos, begin, end)`.
void array_list_vinsert(void **list, void *at, void *begin, void *end);
// Clears an arraylist, ensuring the size is `0`. Does not deallocate or resize the arraylist.
void array_list_clear(void *list);
// Voided reserve method. Ensures a minimum capacity in the arraylist, increasing the size if needed.
// If the arraylist already meets or surpasses the new capacity, nothing changes.
// It is recommended that you use the public facing API `array_list_reserve(list, new_cap)`.
void array_list_vreserve(void **list, size_t cap);

// Erases a range inside the arraylist, moving elements down as necessary.
void array_list_erase(void *list, void *begin, void *end);

// Pops an element from the back of the arraylist and returns it. Preserves type info.
#define array_list_pop_back(list)                                                                                      \
    *(typeof(list))array_list_end((void *)list);                                                                       \
    array_list_erase(list, ((typeof(list))array_list_vend((void *)list)) - 1, array_list_vend((void *)list));

// Creates a new arraylist given a desired type.
#define array_list_new(type) (type *)array_list_create(sizeof(type), 16)

// Returns the start of the arraylist as a type pointer.
#define array_list_begin(list) ((typeof(list))array_list_vbegin((void *)list))
// Returns the end of the arraylist as a type pointer.
#define array_list_end(list) ((typeof(list))array_list_vend((void *)list))

// Returns the first element of the arraylist, copied.
#define array_list_front(list) (*(typeof(list))array_list_vbegin((void *)list))
// Returns the last element of the arraylist, copied.
#define array_list_back(list) (*(typeof(list))array_list_vend((void *)list))
// Returns a specific element in the arraylist, copied. Same as arraylist[idx]
#define array_list_at(list, idx) (*((typeof(list))array_list_vbegin((void *)list) + (idx)))

// Pushes a value to the end of the arraylist. Must be an lvalue.
#define array_list_push_back(list, value) array_list_vpush_back((void **)(&list), (void *)(&(value)))
// Inserts a range to a specific position in the arraylist.
#define array_list_insert(list, pos, begin, end)                                                                       \
    array_list_vinsert((void **)(&list), (void *)pos, (void *)begin, (void *)end)
// Reserves a minimum capacity in the arraylist.
#define array_list_reserve(list, new_cap) array_list_vreserve((void **)(&list), new_cap)

#endif // SNCL_ARRAYLIST_H__
