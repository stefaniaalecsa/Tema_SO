#include "cat.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        cat(0);
        return 0;
    }

    int ret = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            write(2, "Error: cannot open file\n", 24);
            ret = 1;
            continue;
        }
        cat(fd);
        close(fd);
    }
    return ret;
}
