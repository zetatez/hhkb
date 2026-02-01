# HHKB

Happy Hacking Keyboard! Control Mouse with your Keyboard.

Hints:
- By now, this project works on X11 only.

## Installation

```bash
git clone https://github.com/zetatez/hhkb.git

cd hhkb
sudo make clean install
```

## Configuration

See [*config.c*](https://github.com/zetatez/hhkb/blob/master/config.c).

## Usage

Enter the following command to start `hhkb` in daemon mode.

```bash
hhkb
```

- Use `Alt|Control+n` to enter `norm mode`:
    - use `h/j/k/l` to move around
    - use `U/M/L` to move around same as vim norm `U/M/L` do
    - use `b` to go to the beginning of the current line
    - use `e` to go to the end of the current line
    - use `c` to go to the center of the window
    - use `C` to go to the center of the current line
    - use `v` to start select, press `v` or `Escape` to release
    - use `V` to start select lines, press `v` or `Escape` to release, works on terminal only
    - use `a/s/d/f` to go to the upleft/bottomleft/bottomright/upright of the window
    - press `Escape` to quit the norm mode

![norm mode](https://github.com/zetatez/hhkb/blob/master/demos/norm.gif)

- Use `Alt|Control+h` to enter `hint mode`:
    - enter the (`1st`, `2nd`) coordinates to go anywhere you want
    - press the `Backspace` key to roll back to the full hint when you have pressed the `1st` char
    - press `Escape`  to quit hint mode and escape to the norm mode

![hint mode](https://github.com/zetatez/hhkb/blob/master/demos/hint.gif)

- Use `Alt|Control+c` to enter `cross mode`:
    - use `h/j/k/l` on the crosshair to choose left/down/up/right and keep halving the area
    - press `Escape`  to quit cross mode, and escape to the norm mode, the cursor will show on the center of the last window

![cross mode](https://github.com/zetatez/hhkb/blob/master/demos/cover.gif)

- Use `Alt|Control+r` to restart hhkb: when you have changed the configuration after the hhkb is already started

- Use `Alt|Control+q` to quit hhkb
