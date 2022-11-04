#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start (ap,fmt);
  int d=0;
  char *s,*b=(char *)fmt;
  while (*b!= '\0')
  {
    if (*b!='%')
    {
      *out=*b;
      b++;
      out++;
      continue;
    }
    b++;
    switch (*b)
    {
    case 's':{
      s=va_arg(ap,char *);
      while (*s!='\0')
      {
        *out=*s;
        out++;
        s++;
      }
      b++;
      break;}

    case 'd':{
      d=va_arg(ap,int);
      int digit=0,d1=d;
      do{
        d1=d1/10;
        digit++;
      }
      while (d1>0);
      out+=(digit-1);
      /*for (; digit>0; digit--)
      {
        *out=(char)(d2%10+'0');
        d2=d2/10;
        out--;
      }
      */
      do{
      *out = (char)(d%10 + '0');
      d/=10;
      out--;
      }
      while(d > 0);
        out=out+digit+1;
        b++;
        break;}
    default:{
      *out=*b;
      out++;
      b++;
      break;}
    }
    //b++;
  }
  va_end(ap);
  //out++;
  *out='\0';
  int n=0;
  do{
    n++;
    out++;
  }
  while(*out!='\0');
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
