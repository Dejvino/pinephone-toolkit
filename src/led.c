#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *argv0 = "./led";
static char *led_file_template = "/sys/class/leds/%s:%s/brightness";

void print_usage()
{
    printf("usage: %s COMMAND COLOR [VALUE]\n", argv0);
    printf("  COMMAND = {get, set}\n");
    printf("    get ... prints LED level\n");
    printf("    set ... sets LED level to VALUE\n");
    printf("  COLOR = {red, green, blue, flash}\n");
    printf("    red   ... red:indicator LED\n");
    printf("    green ... green:indicator LED\n");
    printf("    blue  ... blue:indicator LED\n");
    printf("    flash ... white:flash LED\n");
    printf("  VALUE = [0..255] ... light level\n");
}

void print_usage_and_fail()
{
    print_usage();
    exit(EXIT_FAILURE);
}

void command_get(char *filepath)
{
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for reading.\n", filepath);
        exit(EXIT_FAILURE);
    }
    int buffer_length = 255;
    char buffer[buffer_length];
    while (fgets(buffer, buffer_length, fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
}

void command_set(char *filepath, int value)
{
    if (setuid(0)) {
        fprintf(stderr, "Cannot set root permissions via setuid(0).\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(filepath, "w+");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for writing.\n", filepath);
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
    if (argc < 3) {
        print_usage_and_fail();
    }
    char *command = argv[1];
    char *led = argv[2];
    if (strcmp(led, "red") != 0
        && strcmp(led, "blue") != 0
        && strcmp(led, "green") != 0
        && strcmp(led, "flash")) {
        print_usage_and_fail();
    }
    char *type = "indicator";
    if (strcmp(led, "flash") == 0) {
        led = "white";
        type = "flash";
    }
    char filename[255];
    sprintf(filename, led_file_template, led, type);
    if ((strcmp(command, "get") == 0) && (argc == 3)) {
        command_get(filename);
    } else if ((strcmp(command, "set") == 0) && (argc == 4)) {
        int value = atoi(argv[3]);
        if (value < 0 || value > 255) {
            print_usage_and_fail();
        }
        command_set(filename, value);
    } else {
        print_usage_and_fail();
    }
    exit(EXIT_SUCCESS);
}
