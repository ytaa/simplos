#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

#include <vga.h>

#define KINIT_WELCOME_MSG \
    "\n\n\n\n\n"          \
    "\
              | |  | |    | |                           | |       \n\
              | |  | | ___| | ___ ___  _ __ ___   ___   | |_ ___  \n\
              | |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\  | __/ _ \\ \n\
              \\  /\\  /  __/ | (_| (_) | | | | | |  __/  | || (_) |\n\
               \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|   \\__\\___/ \n\
                                                                  \n\
                                                                  \n\
                          _                 _                     \n\
                         (_)               | |                    \n\
                      ___ _ _ __ ___  _ __ | | ___  ___           \n\
                     / __| | '_ ` _ \\| '_ \\| |/ _ \\/ __|          \n\
                     \\__ \\ | | | | | | |_) | | (_) \\__ \\          \n\
                     |___/_|_| |_| |_| .__/|_|\\___/|___/          \n\
                                     | |                          \n\
                                     |_|                          \n"
#define KINIT_WELCOME_SCREEN_DURATION_MS 3000
#define KINIT_WELCOME_SCREEN_COLOR VGA_COLOR_CYAN

void kinit_display_welcome_screen(void);

#endif