#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"



char *fmtname(char *path) {
    //static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash
    for (p = path+strlen(path); p >= path && *p != '/'; p--);
    p++;

    return p;
}

void find(char *path, char *filename) {
    int fd;
    struct stat st;
    struct dirent de;

    char buf[512], *p;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
        case T_DEVICE:
            // compare name with filename
            if (strcmp(fmtname(path), filename) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }

            strcpy(buf, path);
            
            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    // No inodes in directory
                    continue;
                }
                memmove(p, de.name, DIRSIZ); // append new file name to string
                p[DIRSIZ] = 0; // null terminate string

                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                find (buf, filename);
            }

    }
    close(fd);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }

    char *path = argv[1];
    char *filename = argv[2];

    find(path, filename);
    
    return 0;
}