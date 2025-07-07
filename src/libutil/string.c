#include <libutil/string.h>

void *memset(void *s, int c, word n)
{
    unsigned char *p = s;
    while (n--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void *memcpy(void *dest, const void *src, word n)
{
    unsigned char       *d = dest;
    const unsigned char *s = src;
    while (n--)
    {
        *d++ = *s++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, word n)
{
    unsigned char       *d = dest;
    const unsigned char *s = src;

    if (d < s || d >= s + n)
    {
        // No overlap or src is before dest
        return memcpy(dest, src, n);
    }
    else
    {
        // Overlapping regions, copy backwards
        d += n;
        s += n;
        while (n--)
        {
            *(--d) = *(--s);
        }

        return dest;
    }
}

word strlen(const char *s)
{
    const char *p = s;
    while (*p)
    {
        p++;
    }
    return p - s;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, word n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
    {
        return 0;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
