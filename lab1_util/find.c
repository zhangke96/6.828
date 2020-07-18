#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void get_file_name(char *file_name, const char *dir, const char *name) {
  strcpy(file_name, dir);
  file_name[strlen(dir)] = '/';
  strcpy(file_name + strlen(dir) + 1, name);
}

void find_dir(char *dir, char *to_find) {
  int dir_fd;
  struct stat st;
  struct dirent de;
  char subdir[512] = {0};
  char file_name[512] = {0};
  // 判断文件夹是否存在
  if ((dir_fd = open(dir, 0)) < 0) {
    printf("find: cannot open: %s\n", dir);
    exit();
  }
  if (fstat(dir_fd, &st) < 0) {
    printf("find: cannnot stat: %s\n", dir);
    exit();
  }

  if (st.type != T_DIR) {
    printf("find: %s not dir\n", dir);
    exit();
  }
  while (read(dir_fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0
        || strcmp(de.name, ".") == 0
        || strcmp(de.name, "..") == 0) {
      continue;
    }
    // 判断是否文件夹
    get_file_name(file_name, dir, de.name);
    if (stat(file_name, &st) < 0) {
      printf("find: cannot stat: %s/%s\n", dir, de.name);
      exit();
    }
    if (st.type == T_FILE && strcmp(de.name, to_find) == 0) {
      printf("%s/%s\n", dir, de.name);
    } else if (st.type == T_DIR) {
      // strcpy(subdir, dir);
      // subdir[strlen(dir)] = '/';
      // strcpy(subdir + strlen(dir) + 1, de.name);
      get_file_name(subdir, dir, de.name);
      // printf("find subdir:%s\n", subdir);
      find_dir(subdir, to_find);
    }
  }
}

int main(int argc, char *argv[]) {
  int dir_fd;
  struct stat st;
  if (argc < 2) {
    printf("Usage: %s dir filename\n", argv[0]);
    exit();
  }
  // 判断文件夹是否存在
  if ((dir_fd = open(argv[1], 0)) < 0) {
    printf("find: cannot open: %s\n", argv[1]);
    exit();
  }
  if (fstat(dir_fd, &st) < 0) {
    printf("find: cannnot stat: %s\n", argv[1]);
    exit();
  }

  if (st.type != T_DIR) {
    printf("find: %s not dir\n", argv[1]);
    exit();
  }
  find_dir(argv[1], argv[2]);
  exit();
}
