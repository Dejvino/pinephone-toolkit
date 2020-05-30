project('PinePhone Toolkit', 'c')
install_suid = ['rwsr-xr-x', 0, 0]
executable('pptk-backlight', ['src/backlight.c'],
  install: true, install_mode: install_suid)
executable('pptk-led', ['src/led.c'],
  install: true, install_mode: install_suid)
executable('pptk-cpu-sleep', ['src/cpu-sleep.c'],
  install: true, install_mode: install_suid)