#include <fcntl.h>  //are functie open()
#include <unistd.h> //read(),write(),close()

#define BUF_SIZE 4096 

void cat(int fd) {       //primeste file descriptor
    char buf[BUF_SIZE];  //memoria temporara
    int n;
    while ((n = read(fd, buf, BUF_SIZE)) > 0)  //citim fd in buf in max de bytes dati
        write(1, buf, n);  //scriem n bytes in fd 1 (terminalul)
}

int main(int argc, char *argv[]) {     //argc -nr argm, argv = array argm
    if (argc == 1) {   //nu avem fisier, citim 0
        cat(0);
        return 0;
    }

    int ret = 0;   //0 - a mers
    for (int i = 1; i < argc; i++) {  //iteram prin fisiere
        int fd = open(argv[i], O_RDONLY);   //e read-only, se returneaza un fd
        if (fd < 0) {
            write(2, "Error: Nu se deschide fisierul\n", 24);   //fd returneaza nr et - eroare 
            ret = 1;  //1 - eroare
            continue;
        }
        cat(fd);      //afisam continut
        close(fd);   //inchidem fd
    }

    return ret;
}
