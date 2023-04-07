#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage() { printf("usage: readme [options]\n"); }

char *last_dir(char *path, bool verbose) {
    int i = 0;
    int last_slash = 0;
    char *last_path_buf;
    int last_path_len;

    while (path[i]) {
        if (path[i] == '/') {
            last_slash = i;
        }
        i++;
    }

    last_path_len = (i - last_slash) + 1;
    last_path_buf = malloc(last_path_len * sizeof(char));

    for (int b = 0; b < last_path_len; b++) {
        last_path_buf[b] = path[last_slash + b + 1];
    }

    if (verbose) {
        printf("%s\n", last_path_buf);

        printf("%s\n", path);

        for (int a = 0; a < last_slash; a++) {
            printf(" ");
        }
        printf("^\n");
    }

    return last_path_buf;
}

void create_readme(char *fname, bool verbose) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char *dirname = last_dir(cwd, verbose);
        if (verbose) {
            printf("Trying to create README.md for %s...\n", dirname);
        }

        FILE *f;
        f = fopen("README.md", "w");
        printf("Created README.md for %s!\n", dirname);

        if (f == NULL) {
            printf("Unable to create file.\n");
            exit(1);
        }

        fputs("# ", f);
        fputs(dirname, f);
        fputs("\n", f);
    } else {
        perror("getcwd() error.\n");
    }
}

int main() {
    char *fname = "README.md";
    bool verbose = false;

    if (access(fname, F_OK) == 0) {
        printf("%s already exists.\n", fname);
    } else {
        create_readme(fname, verbose);
    }

    return 0;
}
