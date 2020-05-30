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

## Build & Install
```
$ meson build
$ cd build
$ ninja
$ sudo ninja install
```
