#include <stdlib.h>

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char **data;
    size_t length;
    size_t _capacity;
} ACList;

ACList *ac_list_init(int initial_capacity);

ACList *ac_list_append(ACList *list, const char *str);

/* void array_list_remove(ArrayList *list, int index); */

void ac_list_free(ACList **list);

#ifdef __cplusplus
}

#endif
#endif
