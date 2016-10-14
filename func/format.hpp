#pragma once
#include <string>
#define format__ljustify(s,n,a,b) char s[n];format::ljustify(s,(n),(a),(b))
#define format__rjustify(s,n,a,b) char s[n];format::rjustify(s,(n),(a),(b))
#define format__fill(s,n,a) char s[n];format::fill(s,(n),(a))

namespace format{
  char *ljustify(char *result, int n, std::string str, char fill){
    char *ch = &str[0];
    char *now = result;
    int size = str.size();
    for(int i = 0; i < size; i++) *now++ = *ch++;
    for(int i = size; i < n; i++) *now++ = fill;
    *now = '\0';
    return result;
  }

  char *rjustify(char *result, int n, std::string str, char fill){
    char *ch = &str[0];
    char *now = result;
    int size = n - str.size();
    for(int i = 0; i < size; i++) *now++ = fill;
    for(int i = size; i < n; i++) *now++ = *ch++;
    *now = '\0';
    return result;
  }

  char *fill(char *result, int n, char fill){
    char *now = result;
    for(int i = 0; i < n; i++) *now++ = fill;
    *now = '\0';
    return result;
  }
}
