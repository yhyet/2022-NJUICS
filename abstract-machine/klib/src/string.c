#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //panic("Not implemented");
  int num=0;
  while (*s!='\0')
  {
    s++;
    num++;
  }
  return num;
}

char *strcpy(char *dst, const char *src) {
  //panic("Not implemented");
  char *tmp=dst;
  while (*src!='\0')
  {
    *dst=*src;
    dst++;
    src++;
  }
  *dst='\0';
  return tmp;
}

char *strncpy(char *dst, const char *src, size_t n) {
  //panic("Not implemented");
  size_t i;
  for ( i = 0; i < n && src[i]!='\0'; i++)
  {
    dst[i]=src[i];
  }
  for (; i < n; i++)
  {
    dst[i]='\0';
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  //panic("Not implemented");
  size_t dst_len=strlen(dst);
  size_t i;
  for ( i = 0;src[i]!='\0'; i++)
  {
    dst[dst_len +i]=src[i];
  }
  dst[dst_len +i]='\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  panic("Not implemented");
  if ((s1==NULL)||(s2==NULL))
    return -2;
  while ((*s1!='\0')&&(*s1==*s2))
  {
    s1++;
    s2++;
  }
  if (*s1>*s2)
    return 1;
  else if(*s1<*s2)
    return -1;
  else
    return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  //panic("Not implemented");
  if ((s1==NULL)||(s2==NULL))
    return -2;
  while(n--){
    while ((*s1!='\0')&&(*s1==*s2))
    {
      s1++;
      s2++;
    }
    if (*s1>*s2)
      return 1;
    else if(*s1<*s2)
      return -1;
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  //panic("Not implemented");
  if(s==NULL || n<0){
    return NULL;
  }
  char *pvs =(char *)s;
  while (n-->0)
  {
    *pvs++ = c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  //panic("Not implemented");
  void *ret=dst;
  if (dst<=src || (char*)dst>=((char *)src + n))
  {
    while (n--)
    {
    *(char*)dst= *(char*)src;
    (char*)src++;
    (char*)dst++;
    }
  }
  else {
    dst=(char *)dst+n-1;
    src=(char *)src+n-1;
    while (n--)
    {
      *(char *)dst =*(char *)src;
      dst =(char *)dst -1;
      src =(char *)src -1;
    }
  }
  return ret;
}

void *memcpy(void *out, const void *in, size_t n) {
  //panic("Not implemented");
  assert(out !=NULL);
  assert(in != NULL);
  void *ret =out;
  while (n--)
  {
    *(char*)out= *(char*)in;
    (char*)in++;
    (char*)out++;
  }
  return ret;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  //panic("Not implemented");
    if (((char *)s1==NULL)||((char *)s2==NULL))
    return -2;
  while ((*(char *)s1!='\0')&&(*(char *)s1==*(char *)s2))
  {
    (char *)s1++;
    (char *)s2++;
  }
  if (*(char *)s1>*(char *)s2)
    return 1;
  else if(*(char *)s1<*(char *)s2)
    return -1;
  else
    return 0;
}

#endif
