#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *argv0 = "./backlight";
static char *backlight_file = "/sys/devices/platform/backlight/backlight/backlight/brightness";

void print_usage()
{
    printf("usage: %s COMMAND [VALUE]\n", argv0);
    printf("  COMMAND = {get, set}\n");
    printf("    get ... prints current backlight level\n");
    printf("    set ... sets backlight level to VALUE\n");
    printf("  VALUE = [0..10] ... backlight amount\n");
}

void print_usage_and_fail()
{
    print_usage();
    exit(EXIT_FAILURE);
}

void command_get()
{
    FILE *fp = fopen(backlight_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for reading.\n", backlight_file);
        exit(EXIT_FAILURE);
    }
    int buffer_length = 255;
    char buffer[buffer_length];
    while (fgets(buffer, buffer_length, fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
}

void command_set(int value)
{
    if (setuid(0)) {
        fprintf(stderr, "Cannot set root permissions via setuid(0).\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(backlight_file, "w+");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for writing.\n", backlight_file);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d\n", value);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc < 1) {
        print_usage_and_fail();
    }
    argv0 = argv[0];
    if (argc < 2) {
        print_usage_and_fail();
    }
    char *command = argv[1];
    if ((strcmp(command, "get") == 0) && (argc == 2)) {
        command_get();
    } else if ((strcmp(command, "set") == 0) && (argc == 3)) {
        int value = atoi(argv[2]);
        if (value < 0 || value > 10) {
            print_usage_and_fail();
        }
        command_set(value);
    } else {
        print_usage_and_fail();
    }
    exit(EXIT_SUCCESS);
}
