#define _GNU_SOURCE
#include "cat.h"

#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

void cat(int fd) {
    char buf[BUF_SIZE];
    int n;
    while ((n = read(fd, buf, BUF_SIZE)) > 0)
        write(1, buf, n);
}
