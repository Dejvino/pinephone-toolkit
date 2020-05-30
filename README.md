# PinePhone Toolkit (PPTK)
A collection of tools and utility apps for the PINE64 PinePhone.

## Components
### backlight
Get / Set display backlight brightness.

```
$ pptk-backlight get
4
$ pptk-backlight set 6
```

### led
Get / Set LED light level for the RGB indicator and the white flash light.

```
# Get red RGB indicator brightness
$ pptk-led get red
255
# Turn RED indicator OFF
$ pptk-led set red 0
# Turn FLASH light ON
$ pptk-led set flash 255
```

### cpu-sleep
Puts secondary CPUs to sleep to save energy. Wakes them up to gain more CPU power.
```
$ pptk-cpu-sleep enable
$ pptk-cpu-sleep get
enabled
$ pptk-cpu-sleep disable
$ pptk-cpu-sleep get
disabled
```

### vibrate
Controls the vibrator to play a vibration pattern.
```
# vibrate for 1 second
$ pptk-vibrate 1000

# vibrate for 500 ms, wait for 500 ms, repeat this pattern 5 times
# resulting in an alternating vibration for 5 seconds in total
$ pptk-vibrate 500 500 5

# 6 short vibrations followed by 3 long vibrations
$ pptk-vibrate 300 200 6 && pptk-vibrate 1500 500 3
```

## Build & Install
```
$ meson build
$ cd build
$ ninja
$ sudo ninja install
```
