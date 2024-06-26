#include <stddef.h>

#ifndef AC_STRING_H
#define AC_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *str_arr;
    size_t length;
    size_t _capacity;
} ACString;

ACString *ac_string_init();

/* ACString *ac_string_append(ACString *str1, ACString *str2); */

/* ACString *ac_string_append_char(ACString *str, char c); */

void ac_string_free(ACString **str);

// substring
// replace
// iterator
// Short String Optimization

#ifdef __cplusplus
}

#endif
#endif
