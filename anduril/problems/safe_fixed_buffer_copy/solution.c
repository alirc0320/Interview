#include "solution.h"

/* TODO: implement safe_strcpy and safe_strcat. Do not use strcpy/strcat/
 * strncpy/strncat from string.h for the core logic (strncpy in particular
 * does NOT guarantee NUL-termination — that's the trap this exercise is
 * built around); walking dst/src with your own index or pointer is the
 * point. strlen() is fine to use for locating the end of an existing
 * string in safe_strcat. */


 /* Returns true if all of src (including its terminator) fit within
 * dst_capacity bytes; false if truncated (dst is still NUL-terminated,
 * just shorter than src) or if dst_capacity == 0 (nothing was written). */
bool safe_strcpy(char* dst, size_t dst_capacity, const char* src)
{
    if(src == NULL || dst_capacity == 0)
    {
        return false;
    }

        if(strlen(src) == 0)
        {
            if(dst_capacity != 0)
            {
                dst[0] = '\0';
            }
            return true;
        }

    const size_t src_length = (strlen(src)) + ((size_t)1);

    if (src_length > dst_capacity)
    {
        memcpy(dst, src, dst_capacity);
        dst[dst_capacity - 1] = '\0';
        return false;
    }

    memcpy(dst, src, src_length);

    return true;
}

/* Appends src onto the end of the NUL-terminated string already in dst.
 * Returns true if all of src fit; false if truncated, if dst wasn't
 * already NUL-terminated within dst_capacity, or if dst_capacity == 0. */
bool safe_strcat(char* dst, size_t dst_capacity, const char* src)
{

    if(src == NULL || dst_capacity == 0)
    {
        return false;
    }

    size_t null_termin_idx = 0;

    bool is_null_term = false;

    for(size_t i = 0; i < dst_capacity; i++)
    {
        if(dst[i] == '\0')
        {
            is_null_term = true;
            null_termin_idx = i;
            break;
        }
    }

    if(!is_null_term)
    {
        return false;
    }

    const size_t room_left_in_dest = dst_capacity - null_termin_idx;
    const size_t src_length = strlen(src) + 1;

    if (src_length > room_left_in_dest)
    {
        memcpy(dst + null_termin_idx, src, room_left_in_dest);
        dst[dst_capacity - 1] = '\0';
        return false;
    }

    memcpy(dst + null_termin_idx, src, src_length);

    return true;
}
