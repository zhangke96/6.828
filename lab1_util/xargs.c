#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  // 从标准输入读取作为程序的命令行参数
  char *arg[MAXARG] = {0};  // 指针数据保存参数
  char *single_arg = 0;
  char input;
  int i;
  int cmd_argc = 0;
  int single_arg_length = 0;
  int pid = 0;
  if (argc < 2) {
    printf("usage: %s cmd [args...]", argv[0]);
    exit();
  }
  for (i = 1; i < argc; ++i) {
    arg[i - 1] = argv[i];
  }
  cmd_argc = argc - 1;
  while (read(0, &input, sizeof(char)) == sizeof(char)) {
    if (input == '\n' || input == ' ') {
      if (single_arg) {
        single_arg[single_arg_length] = 0;
        arg[cmd_argc++] = single_arg;
        single_arg = 0;
        single_arg_length = 0;
      }
    } else {
      if (single_arg == 0) {
        single_arg = malloc(512);
        single_arg_length = 0;
      }
      single_arg[single_arg_length++] = input;
    }
  }
  pid = fork();
  if (pid > 0) {
    wait();
  } else {
    exec(arg[0], arg);
  }
  exit();
}
