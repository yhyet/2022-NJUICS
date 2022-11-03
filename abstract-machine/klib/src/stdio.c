#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int reverse(char *a,int len){
  int i=0;
  int t;
  for ( i = 0; i < len/2; i++)
  {
    t=a[i];
    a[i]=a[len-1-i];
    a[len-1-i]=t;
  }
  return 0;
}
void inttostring(int b,char *c){
  char temp[2]={0};
  while (b%10)
  {
    char a=b%10+'0';
    temp[0]=a;
    b=b/10;
    strcat(c,temp);
  }
  reverse(c,strlen(c));

}

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  //panic("Not implemented");
  va_list ap;
  va_start (ap,fmt);
  while (*fmt!= '\0')
  {
    if (*fmt!='%')
    {
      *out=*fmt;
      fmt++;
      out++;
      continue;
    }
    switch (*fmt++)
    {
    case 's':
      char *s;
      s=va_arg(ap,char *);
      while (*s)
      {
        *out=*s;
        out++;
        s++;
      }
      break;

    case 'd':
      int d;
      d=va_arg(ap,int);
      int digit=1,d1=d;
      while (d1/10)
      {
        digit++;
      }
      char s1[128]={0};
      char *p=s1;
      //char *s1;
      inttostring(d,p);
      while (*p)
      {
        *out=*p;
        out++;
        p++;
      }
      break;
    default:
      break;
    }

  }
  *out='\0';
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
