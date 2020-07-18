#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
  int seconds = 0; 
  if (argc <= 1) {   
    printf("usage:%s sleep seconds\n", argv[0]);
    exit();
  }
  seconds = atoi(argv[1]);
  printf("will sleep seconds:%d\n", seconds);
  sleep(seconds);
  exit();
}
