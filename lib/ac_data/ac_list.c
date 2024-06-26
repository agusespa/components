#include "ac_list.h"

#include <stdlib.h>

#define DEFAULT_CAPACITY 10

ACList* ac_list_init(int initial_capacity) {
    if (initial_capacity <= 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    ACList* list = (ACList*)malloc(sizeof(ACList));

    list->data = (char**)malloc(initial_capacity * sizeof(char**));
    list->length = 0;
    list->_capacity = initial_capacity;

    return list;
}

/* StrList* str_list_append(StrList* list, void* element) { */
/*     if (list->length >= list->capacity) { */
/*         /1* resizeDynamicArray(arr); *1/ */
/*     } */
/*     list->data[list->length] = element; */
/*     list->length++; */
/*     return list; */
/* } */

void ac_list_free(ACList** list) {
    if (*list != NULL) {
        free((*list)->data);
        free(*list);
        *list = NULL;
    }
}
