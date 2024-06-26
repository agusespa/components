#include "ac_string.h"

#include <stdlib.h>

ACString* ac_string_init() {
    ACString* ac_str = (ACString*)malloc(sizeof(ACString));

    ac_str->str_arr = (char*)malloc(sizeof(char) * 10);
    ac_str->length = 0;
    ac_str->_capacity = 10;

    return ac_str;
}

void ac_string_free(ACString** str) {
    if (*str != NULL) {
        free((*str)->str_arr);
        free(*str);
        *str = NULL;
    }
}
