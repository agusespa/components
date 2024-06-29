#include "ac_str_builder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ACStrBuilder *ac_sb_init() {
    ACStrBuilder *sb = (ACStrBuilder *)malloc(sizeof(ACStrBuilder));

    sb->str = (char *)malloc(sizeof(char) * 20);
    sb->str[0] = '\0';

    sb->length = 0;
    sb->_capacity = 20;

    return sb;
}

bool _resize_sb_arr(ACStrBuilder *sb, size_t new_capacity) {
    char *new_ptr = realloc(sb->str, new_capacity);
    if (new_ptr == NULL) {
        printf("ERROR: failed to resize array\n");
        return false;
    }

    sb->_capacity = new_capacity;
    sb->str = new_ptr;
    return true;
}

void ac_sb_append_char(ACStrBuilder *sb, char c) {
    size_t len = sb->length;

    if (len == sb->_capacity) {
        if (!_resize_sb_arr(sb, sb->_capacity * 2)) {
            printf(
                "ERROR: failed to append char due to insufficient capacity\n");
            return;
        }
    }

    sb->str[len] = c;
    sb->str[len + 1] = '\0';
    sb->length++;
}

void ac_sb_append_str(ACStrBuilder *sb, const char *str) {
    if (str == NULL) {
        printf("ERROR: Null pointer passed\n");
        return;
    }

    size_t len_to_append = strlen(str);
    size_t len = sb->length;
    size_t required_capacity = sb->length + len_to_append;

    if (required_capacity > sb->_capacity) {
        size_t new_capacity = sb->_capacity;

        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }

        if (!_resize_sb_arr(sb, new_capacity)) {
            printf(
                "ERROR: failed to append char due to insufficient capacity\n");
            return;
        }
    }

    memcpy(sb->str + len, str, len_to_append * sizeof(char));
    sb->length += len_to_append;
    sb->str[sb->length] = '\0';
}

const char *ac_sb_to_string(ACStrBuilder *sb) { return sb->str; }

const char ac_sb_get_char_at(ACStrBuilder *sb, size_t i) {
    if (i > sb->length) {
        printf("ERROR: index out-of-bounds\n");
        return '\0';
    }
    return sb->str[i];
}

const size_t ac_sb_length(ACStrBuilder *sb) { return sb->length; }

void ac_sb_free(ACStrBuilder **sb) {
    if (*sb != NULL) {
        free((*sb)->str);
        free(*sb);
        *sb = NULL;
    }
}
