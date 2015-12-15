#include "xparameters.h"
#include "vga_ip.h"
#include "MenuDraw.h"

// extern int board_state[8][8];
// extern int status;

// ��������������������֣��������ⲿ������ֱ�ӵ������function��������ÿһ��DrawSscore��ʱ�����õ�
// ��ʱ��������������ĸ������������ˡ������Ŷ�������
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

// �����������������������ֵģ��������ⲿ������ֱ�ӵ������function��������ÿһ��DrawSscore��ʱ�����õ�
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

// �ⲿ��Ҫ���õ�������������������Ʒ������򵥷���
void DrawScore(int score, int turn) {
	int num1, num2;
	num1 = score / 10;
	num2 = score % 10;
	EraseNumber(turn); // ÿһ����draw�µ�����֮ǰ����Ҫ�Ѿɵ�����erase��
	if (turn == 1) {
		DrawNumber(num1, 1, 3, turn);
		DrawNumber(num2, 5, 3, turn);
	} else {
		DrawNumber(num1, 1, 13, turn);
		DrawNumber(num2, 5, 13, turn);
	}
}

// ����һ��C��ͼ������Ϊ�´�Ҫ��λ�C������Ϊ����д��һ������
void DrawC() {
	int vga_input;
	vga_input = (1 << 24) + (22 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (1 << 24) + (23 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (1 << 24) + (24 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (1 << 24) + (25 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (1 << 24) + (26 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (2 << 24) + (22 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (2 << 24) + (26 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (3 << 24) + (22 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	vga_input = (3 << 24) + (26 << 16) + (1 << 8) + 3;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
}

// �������Ҳ������Draw�����ڵ�status�ģ������˶�ս�������˻���ս���Լ������Ѷȵȣ�
void DrawStatus(int status) {
	int i, j;
	int vga_input;
	for (i = 0; i < 10; i++) { // ��һ������յ�ԭ�е�status��ʾ
		for (j = 21; j < 29; j++) {
			vga_input = (i << 24) + (j << 16) + (1 << 8) + 2;
			VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		}
	}
	if (status == 0) {
		// �ڴ˴��������0�Ĵ��룬��ʾStopֹͣ
		DrawNumber(0, 4, 22, 3);
	} else if (status == 1) {
		// �ڴ˴��������C1�Ĵ��룬��ʾEasyģʽ
		DrawC();
		DrawNumber(1, 5, 22, 3);
	} else if (status == 2) {
		// �ڴ˴��������C2�Ĵ��룬��ʾmediumģʽ
		DrawC();
		DrawNumber(2, 5, 22, 3);
	} else if (status == 3) {
		// �ڴ˴��������C3�Ĵ��룬��ʾhardģʽ
		DrawC();
		DrawNumber(3, 5, 22, 3);
	} else if (status == 4) {
		// �ڴ˴��������P�Ĵ��룬��ʾ�˻���սģʽ
		vga_input = (4 << 24) + (22 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (4 << 24) + (23 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (4 << 24) + (24 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (4 << 24) + (25 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (4 << 24) + (26 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (5 << 24) + (22 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (5 << 24) + (24 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (6 << 24) + (22 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (6 << 24) + (23 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		vga_input = (6 << 24) + (24 << 16) + (1 << 8) + 3;
		VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
	}
}

// ���������������ʼ������һ��menu[]�ģ�����Ҫ��ʼ����ʱ����Ե�����
void DrawMenu() {
	int i, j;
	int vga_input;
	for (i = 0; i < 10; i++) { // ��ʼ����ʾ��������menu����
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
	DrawScore(2, 0); // ��ʼ����ʾ������ķ���
	DrawScore(2, 1); // ��ʼ����ʾ������ķ���
}
