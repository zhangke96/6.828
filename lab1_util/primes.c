#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int is_prime(int number) {
  int result = 1;
  for (int i = 2; i < number - 1; ++i) {
    if (number % i == 0) {
      result = 0;
      break;
    }
  }
  return result;
}

void read_and_handle(int fd) {
  int read_ret, number;
  int has_prime = 0;
  int child_pid = 0;
  int pipe_fd[2] = {-1, -1};
  while ((read_ret = read(fd, &number, sizeof(int))) == sizeof(int)) {
    if (!has_prime) {
      if (is_prime(number)) {
        printf("prime %d, pid:%d\n", number, getpid());
        has_prime = 1;
        continue;
      }
    } else {
      // 将数据发送给子进程
      if (child_pid == 0) {
        pipe(pipe_fd);
        child_pid = fork();
        if (child_pid > 0) {
          close(pipe_fd[0]);
        } else if (child_pid == 0) {
          close(pipe_fd[1]);
          read_and_handle(pipe_fd[0]);
          close(pipe_fd[0]);
        }
      }
      write(pipe_fd[1], &number, sizeof(int));
    }
  }
  if (pipe_fd[1] >= 0) {
    close(pipe_fd[1]);
  }
  close(fd);
  exit();
}

int main(int argc, char *argv[]) {
  int pipe_fd[2];
  int pid = -1;
  pipe(pipe_fd);
  pid = fork();
  if (pid > 0) {
    // 父进程
    close(pipe_fd[0]);
    for (int i = 2; i <= 35; ++i) {
      write(pipe_fd[1], &i, sizeof(int));
    }
    close(pipe_fd[1]);
    exit();
  } else if (pid == 0) {
    close(pipe_fd[1]);
    read_and_handle(pipe_fd[0]);
  }
  exit();
}
