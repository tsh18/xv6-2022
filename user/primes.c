#include "kernel/types.h"
#include "user/user.h"

void print_prime(int fd) {
  int prime;
  int p[2];
  int read_num;
  int num;

  pipe(p);

  if (read(fd, &prime, sizeof(int)) < 1) {
    exit(1);
  }
  printf("prime %d\n", prime);

  int pid = fork();
  if (pid == 0) {
    close(p[1]);
    print_prime(p[0]);
  }
  while ((read_num = read(fd, &num, sizeof(int))) > 0) {
    if (num % prime != 0) {
      write(p[1], &num, sizeof(int));
    }
  }
  close(p[1]);
  close(fd);
  wait(0);
  exit(0);
}

int main(int argc, char *argv[]) {
  int p[2];

  pipe(p);
  int pid = fork();
  if (pid == 0) {
    close(p[1]);
    print_prime(p[0]);
  }
  close(p[0]);
  for (int i = 2; i < 36; i++) {
    write(p[1], &i, sizeof(int));
  }
  close(p[1]);
  wait(0);
  exit(0);
}