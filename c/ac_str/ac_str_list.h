#include <stdbool.h>
#include <stdlib.h>

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char **str_arr;
    size_t length;
    size_t _capacity;
} ACStrList;

ACStrList *ac_str_list_init(size_t initial_capacity);

void ac_str_list_append(ACStrList *sl, char **str_ptr);

void ac_str_list_replace(ACStrList *sl, char **str_ptr, size_t i);

const char *ac_str_list_get_str_at(ACStrList *sl, size_t i);

const size_t ac_str_list_length(ACStrList *sl);

void ac_str_list_remove(ACStrList *sl, size_t i);

void ac_str_list_free(ACStrList **sl);

bool _resize_sl_arr(ACStrList *sl, size_t new_capacity);

#ifdef __cplusplus
}

#endif
#endif
