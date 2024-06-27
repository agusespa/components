#include <stdbool.h>
#include <stddef.h>

#ifndef AC_SB_H
#define AC_SB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *str_arr;
    size_t length;
    size_t _capacity;
} ACStringBuilder;

ACStringBuilder *ac_sb_init();

ACStringBuilder *ac_sb_append_char(ACStringBuilder *sb, char c);

/* ACString *ac_string_append(ACString *str1, ACString *str2); */

void ac_sb_free(ACStringBuilder **str);

// substring
// replace
// iterator
// Short String Optimization

bool _resize_sb_arr(ACStringBuilder *sb);

#ifdef __cplusplus
}

#endif
#endif
