//#include "../src/intf/screen.h"
#include "screen.h"
// #include "../cpu/ports.h"
#include "ports.h"
// #include "../libc/mem.h"
#include "mem.h"
// #include "../libc/string.h"
#include "string.h"

// Declaration of private functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_column(int offset);

/***********************************
 * Public kernel API functions     *
 ***********************************/

/* Print a message at specified location
 * If col, row are negative, use current offset
 */
void kprint_at(char* message, int col, int row) 
{
	// set cursor if col, row are negative
	int offset;

	if (col >= 0 && row >= 0) {
		offset = get_offset(col, row);
	} else {
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_column(offset);
	}

	// Loop through message and print it
	int i = 0;

	while(message[i] != '\0') {
		offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
		// Compute row/col for next iteration
		row = get_offset_row(offset);
		col = get_offset_column(offset);
	}

}


void kprint(char* message) 
{
	kprint_at(message, -1, -1);
}

void kprint_int(int data) 
{
	char msg[1024];
	int_to_ascii(data,msg);
	kprint(msg);
}

void kprint_backspace() 
{
	int offset = get_cursor_offset() - 2;
	int row = get_offset_row(offset);
	int col = get_offset_column(offset);
	print_char(0x08, col, row, WHITE_ON_BLACK);
}


/************************************
 * Private kernel functions		    *
 ************************************/


/**
 * Innermost print function of our kernel, directly accesses the video memory
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * returns the offset of the next character
 * sets the video cursor to the returned offest
 */

int print_char(char c, int col, int row, char attr) 
{
	unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;	// addr = 0xb8000
	if (!attr) attr = WHITE_ON_BLACK;

	// Error control: print a red 'E' if the coords aren't right
	if(col >= MAX_COLS || row >= MAX_ROWS) {
		vidmem[ 2*(MAX_COLS)*(MAX_ROWS) - 2 ] = 'E';
		vidmem[ 2*(MAX_COLS)*(MAX_ROWS) - 1 ] = RED_ON_WHITE;
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0) offset = get_offset(col, row);
	else offset = get_cursor_offset();

	if (c == '\n') {
		row = get_offset_row(offset);
		offset = get_offset(0, row + 1);
	} else if (c == 0x08) {		// Backspace
		vidmem[offset] = ' ';
		vidmem[offset + 1] = attr;
	} else {
		vidmem[offset] = c;
		vidmem[offset+1] = attr;
		offset += 2;
	}

	/* check if the offset is over screen size and scroll */
	if ( offset >= MAX_ROWS * MAX_COLS * 2 ) {
		int i; 
		for(i = 1; i < MAX_ROWS; i++) {
			memory_copy((char*) (get_offset(0,i) + VIDEO_ADDRESS),
						(char*) (get_offset(0, i - 1) + VIDEO_ADDRESS),
						MAX_COLS * 2 );
		}

		// Blank last line
		char* last_line = (char*) (get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
		for(i = 0; i < MAX_COLS * 2; i++)
			last_line[i] = 0;

		offset -= 2 * MAX_COLS;
	}

	set_cursor_offset(offset);
	return offset;

}


int get_cursor_offset() 
{
	/* Use the VGA ports to get the current cursor position
	 * 1. Ask for high byte of the cursor offset (data 14)
	 * 2. Ask for low byte (data 15)
	 */

	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8; 	// High byte : << 8
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);

	return offset*2;	// Position * size of character cell

}


void set_cursor_offset(int offset) 
{
	// Similar to get_cursor_offset but instead of reding data, we write data
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
	int screen_size = MAX_COLS * MAX_ROWS;
	int i;
	char *screen = (char*)VIDEO_ADDRESS;

	for (i = 0; i < screen_size; i++) {
		screen[i * 2] = ' ';
		screen[i * 2 + 1] = WHITE_ON_BLACK;
	}

	set_cursor_offset(get_offset(0,0));
}


struct Char {
    uint8_t character2;
    uint8_t color2;
};

//struct Char* buffer = (struct Char*) 0xb8000;
size_t col1 = 0;
size_t row1 = 0;
uint8_t color2 = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void print_setcolor(uint8_t foreground, uint8_t background) {
    color2 = foreground + (background << 4);
}

int get_offset(int col, int row) 
{ 
	return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) 
{
	return offset / (2 * MAX_COLS);
}

int get_offset_column(int offset) 
{ 
	return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}