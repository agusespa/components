#include "ac_str_builder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ACStringBuilder *ac_sb_init() {
    ACStringBuilder *sb = (ACStringBuilder *)malloc(sizeof(ACStringBuilder));

    sb->str_arr = (char *)malloc(sizeof(char) * 20);
    sb->str_arr[0] = '\0';

    sb->length = 0;
    sb->_capacity = 20;

    return sb;
}

bool _resize_sb_arr(ACStringBuilder *sb, size_t new_capacity) {
    char *new_ptr = realloc(sb->str_arr, new_capacity);
    if (new_ptr == NULL) {
        printf("ERROR: Failed to resize array\n");
        return false;
    }

    sb->_capacity = new_capacity;
    sb->str_arr = new_ptr;
    return true;
}

ACStringBuilder *ac_sb_append_char(ACStringBuilder *sb, char c) {
    size_t len = sb->length;

    if (len + 1 == sb->_capacity) {
        if (!_resize_sb_arr(sb, sb->_capacity*2)) {
            printf(
                "ERROR: failed to append char due to insufficient capacity\n");
            return sb;
        }
    }

    sb->str_arr[len] = c;
    sb->str_arr[len + 1] = '\0';
    sb->length = ++len;

    return sb;
}

/* ACStringBuilder *ac_sb_append_str(ACStringBuilder *sb, char *str) { */
/*     if (str == NULL) { */
/*         printf("Error: Null pointer passed\n"); */
/*         return sb; */
/*     } */

/*     size_t len_to_append = strlen(str); */
/*     if (str[len_to_append] != '\0') { */
/*         printf("ERROR: Non-null-terminated string passed\n"); */
/*         return sb; */
/*     } */

/*     size_t len = sb->length; */
/*     size_t required_capacity = sb->length + len_to_append + 1; */
/*     size_t new_capacity = sb->_capacity; */

/*     if (len_to_append + 1 == sb->_capacity) { */
/*         while (sb->_capacity < required_capacity) { */
/*             new_capacity *= 2; */
/*         } */
/*         if (!_resize_sb_arr(sb, new_capacity)) { */
/*             printf( */
/*                 "ERROR: failed to append char due to insufficient capacity\n"); */
/*             return sb; */
/*         } */
/*     } */

/*     sb->str_arr[len] = c; */
/*     sb->str_arr[len + 1] = '\0'; */
/*     sb->length = ++len; */

/*     return sb; */
/* } */

const char *ac_sb_to_string(ACStringBuilder *sb) { return sb->str_arr; }

char ac_sb_get_char_at(ACStringBuilder *sb, size_t i) {
    if (i > sb->length) {
        printf("Index out-of-bounds\n");
        return '\0';
    }
    return sb->str_arr[i];
}

const size_t ac_sb_length(ACStringBuilder *sb) { return sb->length; }

void ac_sb_free(ACStringBuilder **sb) {
    if (*sb != NULL) {
        free((*sb)->str_arr);
        free(*sb);
        *sb = NULL;
    }
}
