#include <stdio.h>
#include <string.h>

char *get_time() {
  char *pts; /* pointer to time string */
	time_t now; /* current time */
	char *ctime();
	(void) time(&now);
  char *result = ctime(&now);
  if (result[strlen(result)-1] == '\n' )
    result[strlen(result)-1] = 0;
  return result;
}

int main() {
  	printf("%s", get_time());

   return(0);
}
