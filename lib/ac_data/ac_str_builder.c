#include "ac_str_builder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ACStringBuilder *ac_sb_init() {
    ACStringBuilder *sb = (ACStringBuilder *)malloc(sizeof(ACStringBuilder));

    sb->str_arr = (char *)malloc(sizeof(char) * 20);
    sb->str_arr[0] = '\0';

    sb->length = 0;
    sb->_capacity = 20;

    return sb;
}

bool _resize_sb_arr(ACStringBuilder *sb) {
    size_t new_capacity = sb->_capacity + 20;

    char *new_ptr = realloc(sb->str_arr, new_capacity);
    if (new_ptr == NULL) {
        printf("Failed to resize array.\n");
        return false;
    }

    sb->_capacity = new_capacity;
    sb->str_arr = new_ptr;
    return true;
}

ACStringBuilder *ac_sb_append_char(ACStringBuilder *sb, char c) {
    size_t len = sb->length;

    if (len + 1 == sb->_capacity) {
        if (!_resize_sb_arr(sb)) {
            printf("Failed to append char: insufficient capacity.\n");
            return sb;
        }
    }

    sb->str_arr[len] = c;
    sb->str_arr[len + 1] = '\0';
    sb->length = ++len;

    return sb;
}

void ac_sb_free(ACStringBuilder **sb) {
    if (*sb != NULL) {
        free((*sb)->str_arr);
        free(*sb);
        *sb = NULL;
    }
}
