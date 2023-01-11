#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void int2char(int d,int digit,char *a){
  //char a[128]={};
  a[digit]='\0';
  for (int i = digit-1; i >=0; i--)
  {
    a[i]=d%10;
    d=d/10;
  }
}

int makestr(char *out,const char *fmt,va_list ap){
    int d=0;
  char *s;
  while (*fmt!= '\0')
  {
    if (*fmt!='%')
    {
      *out=*fmt;
      fmt++;
      out++;
      continue;
    }
    fmt++;
    switch (*fmt)
    {
    case 's':{
      s=va_arg(ap,char *);
      while (*s!='\0')
      {
        *out=*s;
        out++;
        s++;
      }
      fmt++;
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
        fmt++;
        break;}
    default:{
      *out=*fmt;
      out++;
      fmt++;
      break;}
    }
    //fmt++;
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

int printf(const char *fmt, ...) {
  //panic("Not implemented");
  va_list ap;
  va_start (ap,fmt);
  char out[4096]={};
  int n=makestr(out,fmt,ap);
  char *putchp=out;
  while (*putchp!='\0')
  {
    putch(*putchp);
    putchp++;
  }
  va_end(ap);
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start (ap,fmt);
  return makestr(out,fmt,ap);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
