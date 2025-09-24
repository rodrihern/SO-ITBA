#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAXLEN 1024

void print_directory(const char * path, int level);

int main(int argc, char * argv[]) {
    if (argc != 2) {
        puts("usage: tree <directory>");
        return 1;
    }

    print_directory(argv[1], 1);
    return 0;
}

void print_directory(const char * path, int level) {
    DIR * dir = opendir(path);
    if (!dir) {
        perror("failed to open directory");
        return;
    }
    struct dirent * entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char full_path[PATH_MAXLEN];
        snprintf(full_path, PATH_MAXLEN, "%s/%s", path, entry->d_name);
        struct stat file_stat;
        stat(full_path, &file_stat);
        char type = S_ISDIR(file_stat.st_mode) ? 'd' : 'f';
        // printeamos el amigo
        putchar(type);
        for (int i = 0; i < level; i++) {
            putchar('\t');
        }
        puts(entry->d_name);
        // checkeamos si es un directorio y llamamos recursivamente
        if (type == 'd') {
            print_directory(full_path, level + 1);
        }
    }

    closedir(dir);
}


