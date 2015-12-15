#include "xparameters.h"
#include "vga_ip.h"
#include "MenuDraw.h"

// extern int board_state[8][8];
// extern int status;

// 这个函数是用来绘制数字，我们在外部并不会直接调用这个function，而是在每一次DrawSscore的时候会调用到
// 当时发明了这个函数的哥们真是辛苦了……看着都觉得累
void DrawNumber(int number, int x_pos, int y_pos, int turn) {
	int vga_input;

	switch (number) {
	case 0:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 1:
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 2:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 3:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 4:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 5:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 6:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 7:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 8:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	case 9:
		vga_input = (x_pos << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + (y_pos << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 1) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 2) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 3) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (x_pos << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 1) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = ((x_pos + 2) << 24) + ((y_pos + 4) << 16) + (1 << 8) + turn;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		break;
	}

}

// 这个函数是用来清空已有数字的，我们在外部并不会直接调用这个function，而是在每一次DrawSscore的时候会调用到
void EraseNumber(int turn) {
	int i, j;
	int vga_input;
	if (turn == 1) {
		for (i = 0; i < 10; i++) {
			for (j = 1; j < 10; j++) {
				vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
				VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
			}
		}
	} else {
		for (i = 0; i < 10; i++) {
			for (j = 11; j < 20; j++) {
				vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
				VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
			}
		}
	}
}

// 外部主要调用的是这个函数，用来绘制分数，简单方便
void DrawScore(int score, int turn) {
	int num1, num2;
	num1 = score / 10;
	num2 = score % 10;
	EraseNumber(turn); // 每一次在draw新的数字之前，都要把旧的数字erase掉
	if (turn == 1) {
		DrawNumber(num1, 1, 3, turn);
		DrawNumber(num2, 5, 3, turn);
	} else {
		DrawNumber(num1, 1, 13, turn);
		DrawNumber(num2, 5, 13, turn);
	}
}

// 这个函数也是用来Draw出现在的status的（是人人对战，还是人机对战，以及何种难度等）
// 但是这个功能现在可能来不及做了，所以看情况吧
void DrawStatus(int status) {
	int i, j;
	int vga_input;
	for (i = 0; i < 10; i++) { // 第一步先清空掉原有的status显示
		for (j = 21; j < 29; j++) {
			vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
			VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		}
	}
	if (status == 1) {
		// 在此处放入绘制PVP的代码，表示人机对战模式
	} else if (status == 2) {
		// 在此处放入绘制E的代码，表示Easy模式
	} else if (status == 3) {
		// 在此处放入绘制M的代码，表示medium模式
	} else {
		// 在此处放入绘制H的代码，表示hard模式
	}

}

// 这个函数是用来初始化画出一个menu[]的，在想要初始化的时候可以调用下
void DrawMenu() {
	int i, j;
	int vga_input;
	for (i = 0; i < 10; i++) { // 初始化显示出基本的menu界面
		vga_input = (i << 24) + (0 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (i << 24) + (10 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (i << 24) + (20 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (i << 24) + (29 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		for (j = 1; j < 10; j++) {
			vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
			VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		}
		for (j = 11; j < 20; j++) {
			vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
			VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		}
		for (j = 21; j < 29; j++) {
			vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
			VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		}
	}
	DrawScore(2, 0); // 初始化显示出白棋的分数
	DrawScore(2, 1); // 初始化显示出黑棋的分数
}
