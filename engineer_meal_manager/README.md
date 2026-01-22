## Welcome to Meal Manager!

## Running this project
### On Ubuntu 18.04
1. Open the Ubuntu terminal.
2. Run `gcc -o 1_meal_manager/run_meal_manager.out 1_meal_manager/meal_manager.c` to compile the code and then it will create `run_meal_manager.out`.
3. Run `run_meal_manager.out` to run the program.

## Some errors while running this project
### 1. `conio.h: No such file or directory` on Ubuntu 18.04
Use `#include <curses.h>` instead of `#include <conio.h>`, more detail [here](https://stackoverflow.com/questions/8792317/where-is-the-conio-h-header-file-on-linux-why-cant-i-find-conio-h) </br>
Run `sudo apt-get install libncurses5-dev libncursesw5-dev` to install `curses.h` </br>

### 2. 