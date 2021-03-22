#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>

static char *argv0 = "./vibrate";
static char *vibrator_file = "/dev/input/by-path/platform-vibrator-event";

void print_usage()
{
    printf("usage: %s LENGTH [DELAY] [COUNT]\n", argv0);
    printf("  LENGTH = [0..] ... duration of vibration (in ms)\n");
    printf("  DELAY = [0..]  ... delay before vibration is repeated (in ms)\n");
    printf("  COUNT = [1..]  ... how many times the vibration pattern is to be repeated\n");
}

void print_usage_and_fail()
{
    print_usage();
    exit(EXIT_FAILURE);
}

void assert(bool success, char* error_message)
{
    if (success) {
        return;
    }
    fprintf(stderr, "%s\n", error_message);
    exit(EXIT_FAILURE);
}

void write_event(int fd, int e_id, int value)
{
    struct input_event event;
    memset(&event, 0, sizeof(event));
    event.type = EV_FF;
    event.code = e_id;
    event.value = value;
    int res = write(fd, &event, sizeof event);
    assert(res >= 0, "write event failed");
}

void vibrate(int length, int delay, int repeat)
{
    int res;
    
    res = setuid(0);
    assert(res == 0, "Cannot set root permissions via setuid(0).");

    int fd = open(vibrator_file, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        fprintf(stderr, "Cannot open file '%s' for writing.\n", vibrator_file);
        exit(EXIT_FAILURE);
    }

    int effects;
    res = ioctl(fd, EVIOCGEFFECTS, &effects);
    assert(res >= 0, "ioctl(EVIOCGEFFECTS) failed");
    assert(effects > 0, "not enough simultaneous effects supported");

    /* upload rumble effect */
    struct ff_effect e;
    memset(&e, 0, sizeof(e));
    e.type = FF_RUMBLE,
    e.id = -1;
    e.replay.length = length;
    e.replay.delay = delay;
    e.u.rumble.strong_magnitude = 0xc000;
    e.u.rumble.weak_magnitude = 0;
    res = ioctl(fd, EVIOCSFF, &e);
    assert(res >= 0, "ioctl(EVIOCSFF) failed");

    /* play vibration pattern */
    write_event(fd, e.id, repeat);
    usleep((length + delay) * repeat * 1000);
    write_event(fd, e.id, 0);

    /* clear rumble effect */
    res = ioctl(fd, EVIOCRMFF, e.id);
    assert(res >= 0, "ioctl(EVIOCRMFF) failed");

    close(fd);
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
    int length = atoi(argv[1]);
    int delay = argc < 3 ? 0 : atoi(argv[2]);
    int repeat = argc < 4 ? 1 : atoi(argv[3]);
    if (length < 0 || delay < 0 || repeat <= 0) {
        print_usage_and_fail();
    }
    //printf("vibrate %d ms, wait %d ms, repeat %d\n", length, delay, repeat);
    vibrate(length, delay, repeat);
    exit(EXIT_SUCCESS);
}
