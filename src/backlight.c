#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

static char *argv0 = "./backlight";
static char *backlight_file = "/sys/devices/platform/backlight/backlight/backlight/brightness";
static char *max_backlight_file = "/sys/devices/platform/backlight/backlight/backlight/max_brightness";

int max_brightness = 10;

void print_usage()
{
    printf("usage: %s COMMAND [VALUE]\n", argv0);
    printf("  COMMAND = {get, set}\n");
    printf("    get         ... prints current raw backlight level\n");
    printf("    get_percent ... prints current percentage backlight level\n");
    printf("    set         ... sets backlight level to raw VALUE_RAW\n");
    printf("    set_percent ... sets backlight level to percentage VALUE_PCT\n");
    printf("  VALUE_RAW = [0..%d]\n", max_brightness);
    printf("  VALUE_PCT = [0..100]\n");
}

void print_usage_and_fail()
{
    print_usage();
    exit(EXIT_FAILURE);
}

void read_max_brightness()
{
    FILE *fp = fopen(max_backlight_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for reading. Using the default max brightness '%d'.\n",
		max_backlight_file, max_brightness);
	return;
    }
    fscanf(fp, "%d", &max_brightness);
    fclose(fp);
}

void command_get(bool percent)
{
    FILE *fp = fopen(backlight_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file '%s' for reading.\n", backlight_file);
        exit(EXIT_FAILURE);
    }
    int buffer_length = 255;
    char buffer[buffer_length];
    fgets(buffer, buffer_length, fp);
    if (percent) {
        int value = atoi(buffer);
        printf("%d\n", value * 100 / max_brightness);
    } else {
        printf("%s", buffer);
    }
    fclose(fp);
}

void command_set(int value, bool percent)
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
    int raw = percent ? (value * max_brightness / 100) : value;
    fprintf(fp, "%d\n", raw);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    read_max_brightness();
    if (argc < 1) {
        print_usage_and_fail();
    }
    argv0 = argv[0];
    if (argc < 2) {
        print_usage_and_fail();
    }
    char *command = argv[1];
    if ((strcmp(command, "get") == 0) && (argc == 2)) {
        command_get(false);
    } else if ((strcmp(command, "get_percent") == 0) && (argc == 2)) {
        command_get(true);
    } else if ((strcmp(command, "set") == 0) && (argc == 3)) {
        int value = atoi(argv[2]);
        if (value < 0 || value > max_brightness) {
            print_usage_and_fail();
        }
        command_set(value, false);
    } else if ((strcmp(command, "set_percent") == 0) && (argc == 3)) {
        int value = atoi(argv[2]);
        if (value < 0 || value > 100) {
            print_usage_and_fail();
        }
        command_set(value, true);
    } else {
        print_usage_and_fail();
    }
    exit(EXIT_SUCCESS);
}

