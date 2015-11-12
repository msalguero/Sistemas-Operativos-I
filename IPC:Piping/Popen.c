#include <stdio.h>


int PATH_MAX = 20;

FILE *fp;
int status;
char path[PATH_MAX];


fp = popen("ls *", "r");
if (fp == NULL)
    exit();

while (fgets(path, PATH_MAX, fp) != NULL)
    printf("%s", path);


status = pclose(fp);
if (status == -1) {
    /* Error reported by pclose() */

} else {
    /* Use macros described under wait() to inspect `status' in order
       to determine success/failure of command executed by popen() */

}
