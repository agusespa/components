#include <stdbool.h>
#include <stddef.h>

#ifndef AC_SB_H
#define AC_SB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *str;
    size_t length;
    size_t _capacity;
} ACStrBuilder;

ACStrBuilder *ac_sb_init();

void ac_sb_append_char(ACStrBuilder *sb, char c);

void ac_sb_replace_char(ACStrBuilder *sb, char c, size_t i);

void ac_sb_append_str(ACStrBuilder *sb, const char *str);

const char *ac_sb_to_string(ACStrBuilder *sb);

char *ac_sb_substring(ACStrBuilder *sb, size_t start, size_t end);

const char ac_sb_get_char_at(ACStrBuilder *sb, size_t i);

const size_t ac_sb_length(ACStrBuilder *sb);

void ac_sb_free(ACStrBuilder **sb);

// TODO Short String Optimization

bool _resize_sb_arr(ACStrBuilder *sb, size_t new_capacity);

#ifdef __cplusplus
}

#endif
#endif
