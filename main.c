#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define USERNAME "JakeRoggenbuck"
#define WORKFLOW_NAME "build"

void add_build_flag(char *output, int n, char *username, char *project_name) {

    int proj_len = strlen(project_name);
    int uname_len = strlen(username);

    if (n < proj_len + uname_len + 146) {
        printf("Not enough space in build flag string.\n");
        exit(1);
    }

    sprintf(output,
            "[![Build](https://img.shields.io/github/actions/workflow/status/"
            "%s/%s/%s.yml?branch=main&style=for-the-badge)](https://"
            "github.com/%s/%s/actions)",
            username, project_name, WORKFLOW_NAME, username, project_name);
}

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

void create_readme(char *fname, bool verbose, bool should_add_build_flag) {
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

        if (should_add_build_flag) {
            fputs("\n", f);

            char build_flag[512];
            add_build_flag(build_flag, 512, USERNAME, dirname);

            fputs(build_flag, f);
            fputs("\n", f);
        }
    } else {
        perror("getcwd() error.\n");
    }
}

int main() {
    char *fname = "README.md";
    bool verbose = false;
    bool should_add_build_flag = true;

    if (access(fname, F_OK) == 0) {
        printf("%s already exists.\n", fname);
    } else {
        create_readme(fname, verbose, should_add_build_flag);
    }

    return 0;
}
