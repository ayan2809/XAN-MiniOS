#include "print.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    //print_str("Welcome to our 64-bit kernel!");
    print_str("||      ||  |||||||||  ||||      ||       OOOOOOO   OOOOOOOOO\n");
    print_str(" ||    ||   ||     ||  || ||     ||      OO     OO  OO\n");
    print_str("  ||  ||    ||     ||  ||  ||    ||      OO     OO  OO\n");
    print_str("    ||      |||||||||  ||   ||   ||      OO     OO  OOOOOOOOO\n");
    print_str("  ||  ||    ||     ||  ||    ||  ||      OO     OO         OO\n");
    print_str(" ||    ||   ||     ||  ||     || ||      OO     OO         OO\n");
    print_str("||      ||  ||     ||  ||      ||||       OOOOOOO   OOOOOOOOO");
}