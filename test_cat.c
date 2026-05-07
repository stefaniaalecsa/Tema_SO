#include "cat.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int passed = 0;
static int failed = 0;

static void check(const char *name, int ok) {
    if (ok) { printf("[PASS] %s\n", name); passed++; }
    else     { printf("[FAIL] %s\n", name); failed++; }
}

static void write_file(const char *path, const char *data, int len) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, data, len);
    close(fd);
}

static int read_file(const char *path, char *buf, int max) {
    int fd = open(path, O_RDONLY);
    int n = read(fd, buf, max - 1);
    if (n < 0) n = 0;
    buf[n] = '\0';
    close(fd);
    return n;
}

int main() {
    char buf[512];
    int fdin, fdout, saved;

    /* Test 1: fisier text simplu */
    write_file("/tmp/t1.txt", "Hello\n", 6);
    fdin  = open("/tmp/t1.txt", O_RDONLY);
    fdout = open("/tmp/out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    saved = dup(1); dup2(fdout, 1);
    cat(fdin);
    dup2(saved, 1); close(saved); close(fdin); close(fdout);
    read_file("/tmp/out.txt", buf, 512);
    check("Text file", strcmp(buf, "Hello\n") == 0);

    /* Test 2: fisier gol */
    write_file("/tmp/empty.txt", "", 0);
    fdin  = open("/tmp/empty.txt", O_RDONLY);
    fdout = open("/tmp/out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    saved = dup(1); dup2(fdout, 1);
    cat(fdin);
    dup2(saved, 1); close(saved); close(fdin); close(fdout);
    read_file("/tmp/out.txt", buf, 512);
    check("Empty file", strcmp(buf, "") == 0);

    /* Test 3: fisier binar */
    char binary[256];
    for (int i = 0; i < 256; i++) binary[i] = (char)i;
    write_file("/tmp/bin.bin", binary, 256);
    fdin  = open("/tmp/bin.bin", O_RDONLY);
    fdout = open("/tmp/out.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    saved = dup(1); dup2(fdout, 1);
    cat(fdin);
    dup2(saved, 1); close(saved); close(fdin); close(fdout);
    int n = read_file("/tmp/out.bin", buf, 512);
    int ok = (n == 256);
    for (int i = 0; i < 256 && ok; i++)
        if (buf[i] != binary[i]) ok = 0;
    check("Binary file", ok);

    /* Test 4: open fisier inexistent returneaza -1 */
    int fd = open("/tmp/nonexistent_xyz.txt", O_RDONLY);
    check("Nonexistent file", fd < 0);
    if (fd >= 0) close(fd);

    printf("Results: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
