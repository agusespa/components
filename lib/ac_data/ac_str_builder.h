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

ACStringBuilder *ac_sb_append_str(ACStringBuilder *sb, const char *str);

const char *ac_sb_to_string(ACStringBuilder *sb);

char ac_sb_get_char_at(ACStringBuilder *sb, size_t i);

const size_t ac_sb_length(ACStringBuilder *sb);

void ac_sb_free(ACStringBuilder **sb);

// TODO
// substring
// replace
// iterator
// Short String Optimization

bool _resize_sb_arr(ACStringBuilder *sb, size_t new_capacity);

#ifdef __cplusplus
}

#endif
#endif
