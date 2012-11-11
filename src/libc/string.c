/*
 * Copyright (c) 2012 Marco Hinz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the author nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stddef.h>

size_t
strlen(char *s)
{
    char *t = s;

    while (*s)
        s++;

    return (size_t)(s - t);
}

char *
strcat(char *dest, const char *src)
{
    if (!dest || !src)
        return NULL;

    for (size_t i = strlen(dest); *src; i++, src++)
        dest[i] = *src;

    return dest;
}

char *
strchr(char *s, const int c)
{
    if (!s)
        return NULL;

    while (*s++)
        if (*s == c)
            return s;

    return NULL;
}

int
strcmp(const char *s1, const char *s2)
{
    int ret;

    if (!s1 || !s2)
        return -1;

    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }

    ret = *(unsigned char *)s1 - *(unsigned char *)s2;

    if (ret < 0)
        ret = -1;
    else if (ret > 0)
        ret = 1;

    return ret;
}

char *
strcpy(char *dest, const char *src)
{
    char *hptr = dest;

    while ((*dest++ = *src++));

    return hptr;
}

char *
strncat(char *dest, const char *src, size_t n)
{
    if (!dest || !src)
        return NULL;

    for (size_t i = strlen(dest); *src && n--; i++, src++)
        dest[i] = *src;

    return dest;
}

int
strncmp(const char *s1, const char *s2, size_t n)
{
    int ret;

    if (!s1 || !s2)
        return -1;

    while (--n)
        if (*s1 && *s1 == *s2) {
            s1++;
            s2++;
        }

    ret = *(unsigned char *)s1 - *(unsigned char *)s2;

    if (ret < 0)
        ret = -1;
    else if (ret > 0)
        ret = 1;

    return ret;
}

char *
strncpy(char *dest, const char *src, size_t n)
{
    char *hptr = dest;

    while (n--)
        *dest++ = *src++;

    return hptr;
}

char *
strrchr(char *s, const int c)
{
    char *tmp = NULL;

    if (!s)
        return NULL;

    while (*s++)
        if (*s == c)
            tmp = s;

    return tmp;
}
