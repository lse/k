/*
* Copyright (c) LSE
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <string.h>

int	memcmp(const void* s1, const void* s2, size_t n)
{
  const char* d1 = s1;
  const char* d2 = s2;

  for (; *d1 == *d2 && n > 0; d1++, d2++, n--)
    continue;

  return (n);
}

void*	memcpy(void* dest, const void* src, size_t n)
{
  const char* s = src;
  char*	d = dest;

  for (; n > 0; n--, d++, s++)
    *d = *s;

  return (dest);
}

void*	memset(void* s, int c, size_t n)
{
  unsigned char* p = NULL;

  for (p = s; n > 0; n--, p++)
    *p = c;

  return (s);
}

int	strcmp(const char* s1, const char* s2)
{
  for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++)
    continue;

  return (*s1 - *s2);
}

char*	strcpy(char* dest, const char* src)
{
  char*	p = NULL;

  for (p = dest; *src != '\0'; p++, src++)
    *p = *src;

  *p = '\0';

  return (dest);
}

char*	strdup(const char* s)
{
  char*	r = NULL;
  char*	p = NULL;

  r = malloc(strlen(s) + 1);

  for (p = r; *s != '\0'; s++, p++)
    *p = *s;

  *p = '\0';

  return (r);
}

size_t	strlen(const char* s)
{
  const char* p = NULL;

  for (p = s; *p != '\0'; p++)
    continue;

  return (p - s);
}

int	strncmp(const char* s1, const char* s2, size_t n)
{
  for (; *s1 == *s2 && *s1 != '\0' && n > 0; s1++, s2++, n--)
    continue;

  return (n);
}
