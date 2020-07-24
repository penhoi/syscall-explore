#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char* argv[])
{
  // Read the /proc/self/status file
  const char *fname = "/proc/self/status";
  FILE *f = fopen(fname, "r");
  if (f == NULL)
  {
    perror("Failed to invoke fopen");
    return -1;
  }

  char *szLine = (char *)malloc(1024);
  size_t len = 1024;

  puts("Information from file /proc/self/status:\n");
  while (getline(&szLine, &len, f) > 0)
    printf("%s", szLine);

  free(szLine);
  fclose(f);

  // invoke getrusage
  puts("\n\nInformation returned by getrusage:\n");
  struct rusage buf;
  getrusage(RUSAGE_SELF, &buf);
  printf("ru_utime: %ld s + %ld us\n", buf.ru_utime.tv_sec, buf.ru_utime.tv_usec);
  printf("ru_stime: %ld s + %ld us\n", buf.ru_stime.tv_sec, buf.ru_stime.tv_usec);

  printf("maxRSS: %ld\n", buf.ru_maxrss);
  printf("minFault: %ld\n", buf.ru_minflt);
  printf("majFault: %ld\n", buf.ru_majflt);

  return 0;
}
