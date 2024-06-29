#include "ac_str_list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 20

ACStrList* ac_str_list_init(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    ACStrList* sl = (ACStrList*)malloc(sizeof(ACStrList));

    sl->str_arr = (char**)malloc(1 * sizeof(char**));
    sl->length = 0;
    sl->_capacity = initial_capacity;

    return sl;
}

bool _resize_sl_arr(ACStrList* sl, size_t new_capacity) {
    char** new_ptr = realloc(sl->str_arr, new_capacity);
    if (new_ptr == NULL) {
        printf("ERROR: failed to resize array\n");
        return false;
    }

    sl->_capacity = new_capacity;
    sl->str_arr = new_ptr;
    return true;
}

void ac_str_list_append(ACStrList* sl, char** str_ptr) {
    if (sl->length == sl->_capacity) {
        if (!_resize_sl_arr(sl, sl->_capacity * 2)) {
            printf(
                "ERROR: failed to append char array due to insufficient "
                "capacity\n");
            return;
        }
    }
    sl->str_arr[sl->length] = *str_ptr;
    sl->length++;
}

const char* ac_str_list_get_str_at(ACStrList* sl, size_t i) {
    if (i > sl->length) {
        printf("ERROR: index out-of-bounds\n");
        return NULL;
    }
    return sl->str_arr[i];
}

void ac_str_list_remove(ACStrList* sl, size_t i) {
    if (i > sl->length) {
        printf("ERROR: index out-of-bounds\n");
        return;
    }
    free(sl->str_arr[i]);

    memcpy(&sl->str_arr[i], &sl->str_arr[i + 1],
           (sl->length - i - 1) * sizeof(char*));

    sl->str_arr[sl->length - 1] = NULL;
    sl->length--;

    if (sl->length < sl->_capacity /2) {
        _resize_sl_arr(sl, sl->_capacity / 2);
    }
}

const size_t ac_str_list_length(ACStrList* sl) { return sl->length; }

void ac_str_list_free(ACStrList** sl) {
    if (*sl != NULL) {
        for (int i = 0; i < (*sl)->length; i++) {
            if ((*sl)->str_arr[i] != NULL) {
                free((*sl)->str_arr[i]);
            }
        }
        free((*sl)->str_arr);
        free(*sl);
        *sl = NULL;
    }
}
