void trim_str(char *s) {
  
    // Two pointers initially at the beginning
  int i = 0, j = 0;

  // Skip leading spaces
  while (s[i] == ' ') i++; 

  // Shift the characters of string to remove
    // leading spaces
  while ((s[j++] = s[i++]));
}
