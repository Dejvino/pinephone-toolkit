#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *argv0 = "./cpu-sleep";
static char *cpu_file_template = "/sys/devices/system/cpu/cpu%d/online";
static int cpu_min = 1;
static int cpu_max = 3;

void print_usage()
{
    printf("usage: %s COMMAND\n", argv0);
    printf("  COMMAND = {get, enable, disable}\n");
    printf("    get     ... prints the current sleep state: 'enabled' or 'disabled'\n");
    printf("    enable  ... enables sleep mode\n");
    printf("    disable ... disables sleep mode\n");
}

void print_usage_and_fail()
{
    print_usage();
    exit(EXIT_FAILURE);
}

void command_get()
{
    char filename[256];
    sprintf(filename, cpu_file_template, 1);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for reading.\n", filename);
        exit(EXIT_FAILURE);
    }
    int buffer_length = 8;
    char buffer[buffer_length];
    fgets(buffer, buffer_length, fp);
    fclose(fp);

    if (strcmp(buffer, "1\n") == 0) {
        printf("disabled\n");
    } else {
        printf("enabled\n");
    }
}

void command_set(int value)
{
    if (setuid(0)) {
        fprintf(stderr, "Cannot set root permissions via setuid(0).\n");
        exit(EXIT_FAILURE);
    }

    for (int cpu = cpu_min; cpu <= cpu_max; cpu++) {
        char filename[256];
        sprintf(filename, cpu_file_template, cpu);
        FILE *fp = fopen(filename, "w+");
        if (fp == NULL) {
            fprintf(stderr, "Cannot open file '%s' for writing.\n", filename);
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "%d\n", value);
        fclose(fp);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 1) {
        print_usage_and_fail();
    }
    argv0 = argv[0];
    if (argc != 2) {
        print_usage_and_fail();
    }
    char *command = argv[1];
    if (strcmp(command, "get") == 0) {
        command_get();
    } else if (strcmp(command, "enable") == 0) {
        command_set(0);
    } else if (strcmp(command, "disable") == 0) {
        command_set(1);
    } else {
        print_usage_and_fail();
    }
    exit(EXIT_SUCCESS);
}
