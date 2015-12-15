#include "xparameters.h"
#include "xgpio.h"
#include "xutil.h"
#include "vga_ip.h"
#include "xps2.h"

#include "MenuDraw.h"

//====================================================
int Board_size = 8;
int Black_num;
int White_num;
int board_state[8][8];

// 在下面的程序中，我们要分清x_cor, y_cor和x_pos, y_pos的区别。
// 其中，x_pos和y_pos是board上的参数，是非常正常的一个int数。代表了棋子所在的某一行某一列
// 而x_cor和y_cor则是x_pos和y_pos经过位运算转换过来的一个data数，是给VGA绘图使用的

// 这个函数是用来绘制整个棋盘的线的
void DrawBoard(void) {
	int i, j, x_cor, y_cor, type, vga_input;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			x_cor = i << 24;
			y_cor = j << 16;
			type = 2;
			vga_input = x_cor + y_cor + type;
			VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
		}
	}
}

// 这个函数是用来绘制每一个旗子的（无旗子的话，变成棋盘本身的颜色？还是变成移动的光标颜色？）
void DrawChess(int x_pos, int y_pos, int turn) {
	int x_cor, y_cor, type, vga_input;
	type = turn;
	x_cor = x_pos << 24;
	y_cor = y_pos << 16;
	vga_input = x_cor + y_cor + type;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
}

// 这个函数的话，是一个专门抹去光标的函数（因为光标要在棋盘上不停移动，在移动到下一个位置时，要把上一个位置的光标还原回棋子颜色）
void EraseCursor(int x_pos, int y_pos, int turn) {
	if (turn == 1 && board_state[x_pos][y_pos] == 4)
		DrawChess(x_pos, y_pos, 5);
	else
		DrawChess(x_pos, y_pos, board_state[x_pos][y_pos]);
}

void EraseLittleChess() {
	int x_pos, y_pos;
	for (x_pos = 0; x_pos < Board_size; x_pos++)
		for (y_pos = 0; y_pos < Board_size; y_pos++)
			if (board_state[x_pos][y_pos] == 4) {
				DrawChess(x_pos, y_pos, 2);
				board_state[x_pos][y_pos] = 2;
			}
}
void DrawAvailable(int turn) {
	int x_pos, y_pos;
	for (x_pos = 0; x_pos < Board_size; x_pos++) //0白1黑2无子3光标4可以下
		for (y_pos = 0; y_pos < Board_size; y_pos++) { //board_state只取0 1 2，4表示可以下
			if (board_state[x_pos][y_pos] == 4 && turn == 0)
				DrawChess(x_pos, y_pos, 4);
			else if (board_state[x_pos][y_pos] == 4 && turn == 1)
				DrawChess(x_pos, y_pos, 5);
		}
}

int IsAvailable(int turn) { // 找出可以落子的位置，画出需调用DrawAvailable() //返回值1为有子可下，返回值为0为无子可下
	int i, j;
	int flip_flag, flip_flag2;
	int result = 0;
	int x_pos, y_pos;
	for (x_pos = 0; x_pos < Board_size; x_pos++) //0白1黑2无子3光标4可以下
		for (y_pos = 0; y_pos < Board_size; y_pos++) //board_state只取0 1 2，4表示可以下
			if (board_state[x_pos][y_pos] == 2) {
				flip_flag = -1;
				if ((x_pos + 1 < Board_size) && (board_state[x_pos + 1][y_pos]
						== (1 - turn)))
					for (i = x_pos + 1; i < Board_size; i++) { // 往右水平方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][y_pos] == 2 || board_state[i][y_pos]
								== 4)
							break;
						else if (board_state[i][y_pos] == turn) { //判断这个方向上是否有可以翻转的棋子
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				if ((x_pos - 1 >= 0) && (board_state[x_pos - 1][y_pos] == (1
						- turn)))
					for (i = x_pos - 1; i >= 0; i--) { // 往左水平方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][y_pos] == 2 || board_state[i][y_pos]
								== 4)
							break;
						else if (board_state[i][y_pos] == turn) { //判断这个方向上是否有可以翻转的棋子
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				if ((y_pos + 1 < Board_size) && (board_state[x_pos][y_pos + 1]
						== (1 - turn)))
					for (i = y_pos + 1; i < Board_size; i++) { // 往上竖直方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[x_pos][i] == 2 || board_state[x_pos][i]
								== 4)
							break;
						else if (board_state[x_pos][i] == turn) {
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				if ((y_pos - 1 >= 0) && (board_state[x_pos][y_pos - 1] == (1
						- turn)))
					for (i = y_pos - 1; i >= 0; i--) { // 往下竖直方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[x_pos][i] == 2 || board_state[x_pos][i]
								== 4)
							break;
						else if (board_state[x_pos][i] == turn) {
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos + 1) < Board_size && (y_pos + 1) < Board_size)
						&& (board_state[x_pos + 1][y_pos + 1] == (1 - turn)))
					for (i = x_pos + 1, j = y_pos + 1; i < Board_size && j
							< Board_size; i++, j++) { // 往东北方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos - 1) >= 0 && (y_pos - 1) >= 0)
						&& (board_state[x_pos - 1][y_pos - 1] == (1 - turn)))
					for (i = x_pos - 1, j = y_pos - 1; i >= 0 && j >= 0; i--, j--) { // 往西南方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos + 1) < Board_size && (y_pos - 1) >= 0)
						&& (board_state[x_pos + 1][y_pos - 1] == (1 - turn)))
					for (i = x_pos + 1, j = y_pos - 1; i < Board_size && j >= 0; i++, j--) { // 往东南方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					//board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos - 1) >= 0 && (y_pos + 1) < Board_size)
						&& (board_state[x_pos - 1][y_pos + 1] == (1 - turn)))
					for (i = x_pos - 1, j = y_pos + 1; i >= 0 && j < Board_size; i--, j++) { // 往西北方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					;
					//board_state[x_pos][y_pos] = 4;
					continue;
				}
			}
	return result;
}
int FindAvailable(int turn) { // 找出可以落子的位置，画出需调用DrawAvailable() //返回值1为有子可下，返回值为0为无子可下
	int i, j;
	int flip_flag, flip_flag2;
	int result = 0;
	int x_pos, y_pos;
	for (x_pos = 0; x_pos < Board_size; x_pos++) //0白1黑2无子3光标4可以下
		for (y_pos = 0; y_pos < Board_size; y_pos++) //board_state只取0 1 2，4表示可以下
			if (board_state[x_pos][y_pos] == 2) {
				flip_flag = -1;
				if ((x_pos + 1 < Board_size) && (board_state[x_pos + 1][y_pos]
						== (1 - turn)))
					for (i = x_pos + 1; i < Board_size; i++) { // 往右水平方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][y_pos] == 2 || board_state[i][y_pos]
								== 4)
							break;
						else if (board_state[i][y_pos] == turn) { //判断这个方向上是否有可以翻转的棋子
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				if ((x_pos - 1 >= 0) && (board_state[x_pos - 1][y_pos] == (1
						- turn)))
					for (i = x_pos - 1; i >= 0; i--) { // 往左水平方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][y_pos] == 2 || board_state[i][y_pos]
								== 4)
							break;
						else if (board_state[i][y_pos] == turn) { //判断这个方向上是否有可以翻转的棋子
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				if ((y_pos + 1 < Board_size) && (board_state[x_pos][y_pos + 1]
						== (1 - turn)))
					for (i = y_pos + 1; i < Board_size; i++) { // 往上竖直方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[x_pos][i] == 2 || board_state[x_pos][i]
								== 4)
							break;
						else if (board_state[x_pos][i] == turn) {
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				if ((y_pos - 1 >= 0) && (board_state[x_pos][y_pos - 1] == (1
						- turn)))
					for (i = y_pos - 1; i >= 0; i--) { // 往下竖直方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[x_pos][i] == 2 || board_state[x_pos][i]
								== 4)
							break;
						else if (board_state[x_pos][i] == turn) {
							flip_flag = i;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos + 1) < Board_size && (y_pos + 1) < Board_size)
						&& (board_state[x_pos + 1][y_pos + 1] == (1 - turn)))
					for (i = x_pos + 1, j = y_pos + 1; i < Board_size && j
							< Board_size; i++, j++) { // 往东北方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos - 1) >= 0 && (y_pos - 1) >= 0)
						&& (board_state[x_pos - 1][y_pos - 1] == (1 - turn)))
					for (i = x_pos - 1, j = y_pos - 1; i >= 0 && j >= 0; i--, j--) { // 往西南方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos + 1) < Board_size && (y_pos - 1) >= 0)
						&& (board_state[x_pos + 1][y_pos - 1] == (1 - turn)))
					for (i = x_pos + 1, j = y_pos - 1; i < Board_size && j >= 0; i++, j--) { // 往东南方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					board_state[x_pos][y_pos] = 4;
					continue;
				}

				flip_flag = -1;
				flip_flag2 = -1;
				if (((x_pos - 1) >= 0 && (y_pos + 1) < Board_size)
						&& (board_state[x_pos - 1][y_pos + 1] == (1 - turn)))
					for (i = x_pos - 1, j = y_pos + 1; i >= 0 && j < Board_size; i--, j++) { // 往西北方向进行判断，看看有多少个同颜色棋子排列
						if (board_state[i][j] == 2 || board_state[i][j] == 4)
							break;
						else if (board_state[i][j] == turn) {
							flip_flag = i;
							flip_flag2 = j;
							result = 1;
							break;
						}
					}
				if (flip_flag != -1) {
					;
					board_state[x_pos][y_pos] = 4;
					continue;
				}
			}
	return result;
}

// 估计在main函数之中，每一次下子完都会调用这个函数，来判断是否有一方获得了胜利（我猜测的）
int check_win() { //0白赢，1黑赢，2平局,-1没有人赢,
	if (Black_num == 0)
		return 0;
	if (White_num == 0)
		return 1;
	if ((IsAvailable(0) == 0 && IsAvailable(1) == 0)
			|| ((Black_num + White_num) == Board_size * Board_size)) {
		if (Black_num > White_num)
			return 1;
		if (Black_num < White_num)
			return 0;
		if (Black_num == White_num)
			return 2;
	}
	return -1;
}
void flip(int x_pos, int y_pos, int turn) { //仅实现翻转功能
	int i, j;
	int flip_flag, flip_flag2, bk_flag;
	flip_flag = -1;
	if ((x_pos + 1 < Board_size) && (board_state[x_pos + 1][y_pos]
			== (1 - turn)))
		for (i = x_pos + 1; i < Board_size; i++) { // 往右水平方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[i][y_pos] == 2 || board_state[i][y_pos] == 4)
				break;
			else if (board_state[i][y_pos] == turn) { //判断这个方向上是否有可以翻转的棋子
				flip_flag = i;
				break;
			}
		}

	//翻转
	if (flip_flag != -1)
		for (i = x_pos + 1; i < flip_flag; i++) {
			board_state[i][y_pos] = turn;
			DrawChess(i, y_pos, turn);//画与turn的颜色覆盖
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}
	flip_flag = -1;
	if ((x_pos - 1 >= 0) && (board_state[x_pos - 1][y_pos] == (1 - turn)))
		for (i = x_pos - 1; i >= 0; i--) { // 往左水平方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[i][y_pos] == 2 || board_state[i][y_pos] == 4)
				break;
			else if (board_state[i][y_pos] == turn) { //判断这个方向上是否有可以翻转的棋子
				flip_flag = i;
				break;
			}
		}

	if (flip_flag != -1)
		for (i = x_pos - 1; i > flip_flag; i--) {
			board_state[i][y_pos] = turn;
			DrawChess(i, y_pos, turn);//画与turn的颜色覆盖
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}

	flip_flag = -1;
	if ((y_pos + 1 < Board_size) && (board_state[x_pos][y_pos + 1]
			== (1 - turn)))
		for (i = y_pos + 1; i < Board_size; i++) { // 往上竖直方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[x_pos][i] == 2 || board_state[x_pos][i] == 4)
				break;
			else if (board_state[x_pos][i] == turn) {
				flip_flag = i;
				break;
			}
		}
	if (flip_flag != -1)
		for (i = y_pos + 1; i < flip_flag; i++) {
			board_state[x_pos][i] = turn;
			DrawChess(x_pos, i, turn);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}
	flip_flag = -1;
	if ((y_pos - 1 >= 0) && (board_state[x_pos][y_pos - 1] == (1 - turn)))
		for (i = y_pos - 1; i >= 0; i--) { // 往下竖直方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[x_pos][i] == 2 || board_state[x_pos][i] == 4)
				break;
			else if (board_state[x_pos][i] == turn) {
				flip_flag = i;
				break;
			}
		}
	if (flip_flag != -1)
		for (i = y_pos - 1; i > flip_flag; i--) {
			board_state[x_pos][i] = turn;
			DrawChess(x_pos, i, turn);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}

	flip_flag = -1;
	flip_flag2 = -1;
	if (((x_pos + 1) < Board_size && (y_pos + 1) < Board_size)
			&& (board_state[x_pos + 1][y_pos + 1] == (1 - turn)))
		for (i = x_pos + 1, j = y_pos + 1; i < Board_size && j < Board_size; i++, j++) { // 往东北方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[i][j] == 2 || board_state[i][j] == 4)
				break;
			else if (board_state[i][j] == turn) {
				flip_flag = i;
				flip_flag2 = j;
				break;
			}
		}
	if (flip_flag != -1)
		for (i = x_pos + 1, j = y_pos + 1; i < flip_flag && j < flip_flag2; i++, j++) {
			board_state[i][j] = turn;
			DrawChess(i, j, turn);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}
	flip_flag = -1;
	flip_flag2 = -1;
	if (((x_pos - 1) >= 0 && (y_pos - 1) >= 0) && (board_state[x_pos - 1][y_pos
			- 1] == (1 - turn)))
		for (i = x_pos - 1, j = y_pos - 1; i >= 0 && j >= 0; i--, j--) { // 往西南方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[i][j] == 2 || board_state[i][j] == 4)
				break;
			else if (board_state[i][j] == turn) {
				flip_flag = i;
				flip_flag2 = j;
				break;
			}
		}
	if (flip_flag != -1)
		for (i = x_pos - 1, j = y_pos - 1; i > flip_flag && j > flip_flag2; i--, j--) {
			board_state[i][j] = turn;
			DrawChess(i, j, turn);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}

	flip_flag = -1;
	flip_flag2 = -1;
	if (((x_pos + 1) < Board_size && (y_pos - 1) >= 0) && (board_state[x_pos
			+ 1][y_pos - 1] == (1 - turn)))
		for (i = x_pos + 1, j = y_pos - 1; i < Board_size && j >= 0; i++, j--) { // 往东南方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[i][j] == 2 || board_state[i][j] == 4)
				break;
			else if (board_state[i][j] == turn) {
				flip_flag = i;
				flip_flag2 = j;
				break;
			}
		}
	if (flip_flag != -1)
		for (i = x_pos + 1, j = y_pos - 1; i < flip_flag && j > flip_flag2; i++, j--) {
			board_state[i][j] = turn;
			DrawChess(i, j, turn);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}
	flip_flag = -1;
	flip_flag2 = -1;
	if (((x_pos - 1) >= 0 && (y_pos + 1) < Board_size) && (board_state[x_pos
			- 1][y_pos + 1] == (1 - turn)))
		for (i = x_pos - 1, j = y_pos + 1; i >= 0 && j < Board_size; i--, j++) { // 往西北方向进行判断，看看有多少个同颜色棋子排列
			if (board_state[i][j] == 2 || board_state[i][j] == 4)
				break;
			else if (board_state[i][j] == turn) {
				flip_flag = i;
				flip_flag2 = j;
				break;
			}
		}
	if (flip_flag != -1)
		for (i = x_pos - 1, j = y_pos + 1; i > flip_flag && j < flip_flag2; i--, j++) {
			board_state[i][j] = turn;
			DrawChess(i, j, turn);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}

}
void Draw_win( win_status) {
	int color;
	DrawBoard();
	if (win_status == 1)
		color = 1;
	else
		color = 0;
	DrawChess(0, 2, color);
	DrawChess(1, 4, color);
	DrawChess(3, 4, color);
	DrawChess(5, 4, color);
	DrawChess(6, 2, color);
	if (win_status == 2)
		color = 1 - color; //如果是平手翻转颜色
	DrawChess(1, 3, color);
	DrawChess(2, 5, color);
	DrawChess(3, 3, color);
	DrawChess(4, 5, color);
	DrawChess(5, 3, color);
}

// 主函数，整个下棋的流程应该都包含其中了
int main(void) {
	XGpio dip; // dip是定义的一个板子类型？
	int dip_check;

	static XPs2 Ps2Inst;
	XPs2_Config *ConfigPtr;
	u32 StatusReg;
	u32 BytesReceived;
	u8 RxBuffer; // RxBuffer这个变量比较重要，在主循环逻辑部分我们需要通过判断这个值来判断到底键盘输入了哪个按键
	int count = 0;
	int win_status = -1;
	int i, j;
	int x_cur = 5, y_cur = 5; // 这个是定义了一个x，y的初始坐标吧
	int turn = 1; // 定义了一个初始的棋子颜色（难道是下子时移动的光标？）

	xil_printf("-- Game Starts! --\r\n"); // 这个应该是在某一个地方输出一段话，方便debug

	XGpio_Initialize(&dip, XPAR_DIP_SWITCHES_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&dip, 1, 0xffffffff);

	ConfigPtr = XPs2_LookupConfig(XPAR_XPS_PS2_0_0_DEVICE_ID);
	XPs2_CfgInitialize(&Ps2Inst, ConfigPtr, ConfigPtr->BaseAddress); // 这一段应该是一些针对板子以及键盘的初始化流程

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board_state[i][j] = 2; // 将board_state[][]这个二维数组里面的所有旗子初始类型都定义为2，也就是无子
		}
	}
	DrawBoard();
	DrawChess(3, 3, 1); // 根据黑白棋的规则，初始化出几个旗子和更新board_state[][]数组
	board_state[3][3] = 1;
	DrawChess(3, 4, 0);
	board_state[3][4] = 0;
	DrawChess(4, 3, 0);
	board_state[4][3] = 0;
	DrawChess(4, 4, 1);
	board_state[4][4] = 1;
	Black_num = 2;
	White_num = 2;
	FindAvailable(turn);
	DrawAvailable(turn);
	DrawChess(x_cur, y_cur, 3); // 画出初始的旗子所在处的颜色，其中3这个参数代表的颜色应该是移动的光标的颜色
	DrawMenu();

	int boardStatus = 4;
	DrawStatus(boardStatus);
	while (1) { // 进入了主循环之中，游戏开始！
		do {
			dip_check = XGpio_DiscreteRead(&dip, 1);
			StatusReg = XPs2_GetStatus(&Ps2Inst);
		} while ((StatusReg & XPS2_STATUS_RX_FULL) == 0);
		BytesReceived = XPs2_Recv(&Ps2Inst, &RxBuffer, 1);
		count = (count + 1) % 3; // 这个count的功能是？防抖吗？似乎对主程序没有逻辑上的影响
		if (count == 1) {
			if (RxBuffer == 0x1D && win_status == -1) { // 0x1D这个值应该对应的是往上移动，也就是W键
				EraseCursor(x_cur, y_cur, turn);
				if (y_cur == 0)
					y_cur = 7;
				else
					y_cur--;
				DrawChess(x_cur, y_cur, 3);
			}
			if (RxBuffer == 0x1B && win_status == -1) { // 0x1B这个值应该对应的是向下移动，也就是S键
				EraseCursor(x_cur, y_cur, turn);
				if (y_cur == 7)
					y_cur = 0;
				else
					y_cur++;
				DrawChess(x_cur, y_cur, 3);
			}
			if (RxBuffer == 0x1C && win_status == -1) { // 0x1C这个值对应的应该是向左移动，也就是A键
				EraseCursor(x_cur, y_cur, turn);
				if (x_cur == 0)
					x_cur = 7;
				else
					x_cur--;
				DrawChess(x_cur, y_cur, 3);
			}
			if (RxBuffer == 0x23 && win_status == -1) { // 0x23这个值对应的应该是向右移动，也就是D键
				EraseCursor(x_cur, y_cur, turn);
				if (x_cur == 7)
					x_cur = 0;
				else
					x_cur++;
				DrawChess(x_cur, y_cur, 3);
			}

			if (RxBuffer == 0x5A) { // 0x5A这个值对应的应该是确定键，也就是enter键
				if (win_status != -1)
					return 0;
				if (board_state[x_cur][y_cur] == 4) {
					EraseLittleChess();
					DrawChess(x_cur, y_cur, turn);
					if (turn == 0) {
						White_num++;
					} else {
						Black_num++;
					}
					board_state[x_cur][y_cur] = turn;
					flip(x_cur, y_cur, turn);
					win_status = check_win();
					if (win_status != -1) { // 若有一方胜利了，本轮游戏结束
						DrawScore(White_num, 0);
						DrawScore(Black_num, 1);
						Draw_win(win_status);//画出显示胜利的棋盘形状
						//						xil_printf("\r\nPlayer %x wins!\r\n", turn + 1);
						//						return 0;

					} else {
						turn = 1 - turn; // 轮换黑白次序
						if (!FindAvailable(turn)) {//一方五子可下时换另一方下
							turn = 1 - turn;
							FindAvailable(turn);
						}
						DrawAvailable(turn);
					}
				}
				DrawScore(White_num, 0);
				DrawScore(Black_num, 1);

			}

			if (RxBuffer == 0x76) { // 0x1C这个值对应的应该是restart，对应的是esc键
				return 0;
				/*for (i = 0; i < 8; i++) {
				 for (j = 0; j < 8; j++) {
				 board_state[i][j] = 2;
				 }
				 }
				 x_cur = 5;
				 y_cur = 5;
				 turn = 1;
				 DrawBoard();
				 DrawChess(3, 3, 1);
				 board_state[3][3] = 1;
				 DrawChess(3, 4, 0);
				 board_state[3][4] = 0;
				 DrawChess(4, 3, 0);
				 board_state[4][3] = 0;
				 DrawChess(4, 4, 1);
				 board_state[4][4] = 1;
				 DrawChess(x_cur, y_cur, 3);
				 Black_num = 2;
				 White_num = 2;
				 FindAvailable(turn);
				 DrawAvailable();
				 xil_printf("\r\nGame Restart!\r\n"); // 以上步骤把棋盘的状态数组board_state[][]又进行了一次初始化*/
			}
		}
	}
}
