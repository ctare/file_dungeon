#ifndef INCLUDE_FNCS
#define INCLUDE_FNCS
#define len_format(s,n,b,c) char s[(n)];length_format(s,(n),(b),(c))
#define len_fill(s,n,c) char s[(n)];length_fill(s,(n),(c))

char *length_format(char *result, int n, std::string str, char fill){
  char *ch = &str[0];
  char *now = result;
  int size = str.size();
  for(int i = 0; i < size; i++) *now++ = *ch++;
  for(int i = size; i < n; i++) *now++ = fill;
  *now = '\0';
  return result;
}

char *length_fill(char *result, int n, char fill){
  char *now = result;
  for(int i = 0; i < n; i++) *now++ = fill;
  *now = '\0';
  return result;
}

#endif
