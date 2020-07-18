#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
  int parent_fd[2];
  int child_fd[2];
  int pid = -1;
  int read_ret = -1;
  pipe(parent_fd);
  pipe(child_fd);
  pid = fork();
  char buffer[1];
  if (pid > 0) {
    // 父进程
    close(parent_fd[0]);
    close(child_fd[1]);
    write(parent_fd[1], "a", 1);
    close(parent_fd[1]);
    read_ret = read(child_fd[0], buffer, 1);
    if (read_ret == 1) {
      printf("%d: received pong\n", getpid());
    } else {
      printf("read_ret:%d\n", read_ret);
    }
    exit();
  } else if (pid == 0) {
    // 子进程
    close(parent_fd[1]);
    close(child_fd[0]);
    read_ret = read(parent_fd[0], buffer, 1);
    if (read_ret == 1) {
      printf("%d: received ping\n",  getpid());
      write(child_fd[1], "b", 1);
      close(child_fd[1]);
    }
    exit();
  } else {
    printf("fork failed\n");
    exit();
  }
}