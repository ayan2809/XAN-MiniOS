#include "print.h"
#include "screen.h"

void kernel_main() {
    print_clear();
    print_setcolor(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    //print_str("Welcome to our 64-bit kernel!");

    //could be done using print_str but it has been commented out in the print.c file too so it cannot be referenced
    //hence i will be using the print str kprint function to print the values
    //print_str("||      ||  ||     ||  ||      ||||       OOOOOOO   OOOOOOOOO\n\n\n");

    kprint("||      ||  |||||||||  ||||      ||       OOOOOOO   OOOOOOOOO\n");
    kprint(" ||    ||   ||     ||  || ||     ||      OO     OO  OO\n");
    kprint("  ||  ||    ||     ||  ||  ||    ||      OO     OO  OO\n");
    kprint("    ||      |||||||||  ||   ||   ||      OO     OO  OOOOOOOOO\n");
    kprint("  ||  ||    ||     ||  ||    ||  ||      OO     OO         OO\n");
    kprint(" ||    ||   ||     ||  ||     || ||      OO     OO         OO\n");
    kprint("||      ||  ||     ||  ||      ||||       OOOOOOO   OOOOOOOOO\n\n\n");

    kprint("Protected Mode enabled\n");
	kprint("Running in kernel space\n");
}