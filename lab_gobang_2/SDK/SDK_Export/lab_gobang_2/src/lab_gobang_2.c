//AI都可悔棋
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
int *p1[60];
int stack[50];
int depth;
const int INFINITE = 99999;
int myflag;

const int EVAL_SCORES[64] = { 500, -150, 30, 10, 10, 30, -150, 500, -150, -250,
		0, 0, 0, 0, -250, -150, 30, 0, 1, 2, 2, 1, 0, 30, 10, 0, 2, 16, 16, 2,
		0, 10, 10, 0, 2, 16, 16, 2, 0, 10, 30, 0, 1, 2, 2, 1, 0, 30, -150,
		-250, 0, 0, 0, 0, -250, -150, 500, -150, 30, 10, 10, 30, -150, 500 };
const int LIBERTY_SCORE = 8;
const int BONUS_SCORE = 30;

void push(int *x) {
	if (depth > 59)
		return;
	p1[depth] = x;
	depth++;
}

int* pop() {
	if (depth < 1)
		return NULL;
	int *r;
	depth--;
	r = p1[depth];
	p1[depth] = NULL;
	return r;
}

void lpush(int s[], int x) {
	if (s[0] >= 50)
		return;
	s[s[0]] = x;
	s[0]++;
}

int lpop(int s[]) {
	if (s[0] <= 1)
		return -1;
	s[0]--;
	return s[s[0]];
}

int find_best1( turn) { //仅仅按照EVAL_SCORES的值选择，选择最大的那个
	int quality = -INFINITE;
	int best_pos = -1;
	int temp_pos = -1;
	int a = 0, b = 0;
	for (a = 0; a < Board_size; a++) {
		for (b = 0; b < Board_size; b++) {
			if (board_state[a][b] == 4) {
				temp_pos = a * Board_size + b;
				if (EVAL_SCORES[temp_pos] > quality) {
					best_pos = temp_pos;
					quality = EVAL_SCORES[best_pos];
				}
			}
		}
	}
	return best_pos;
}
int find_best2(int turn, int ifResult) {
	int quality = -INFINITE;//best quality
	int best_pos = -1;
	int temp_pos = -1;
	int temp_qua = -INFINITE;
	int a, b;
	for (a = 0; a < Board_size; a++) {
		for (b = 0; b < Board_size; b++) {
			if (board_state[a][b] == 4) {
				temp_pos = a * Board_size + b;
				temp_qua = quality_of_pos(temp_pos, turn);
				if (temp_qua >= quality) {
					quality = temp_qua;
					best_pos = temp_pos;
				}
			}
		}
	}
	if (ifResult)
		return quality;
	else
		return best_pos;
}

int find_best3( turn) {
	int quality = -INFINITE;//best quality
	int best_pos = -1;
	int temp_pos = -1;
	int temp_qua = -INFINITE;
	int a, b;
	int avail_pos[50];//all the available positions
	int avail_count = 0;//count of available positions

	// find all the available positions
	for (a = 0; a < Board_size; a++) {
		for (b = 0; b < Board_size; b++) {
			if (board_state[a][b] == 4) {
				temp_pos = a * Board_size + b;
				avail_pos[avail_count] = temp_pos;
				avail_count++;
			}
		}
	}

	//from all the available positions, pick up the best one according to the quality
	for (a = 0; a < avail_count; a++) {
		temp_pos = avail_pos[a];
		temp_qua = quality_of_pos2(temp_pos, turn);
		if (temp_qua >= quality) {
			quality = temp_qua;
			best_pos = temp_pos;
		}
	}
	//best_pos  = 0;
	//if(best_pos<0) DrawChess(0,0,1);
	return best_pos;
}

int quality_of_pos( pos, turn) {
	board_state[pos / Board_size][pos % Board_size] = turn;

	//int result = 0;
	flip(pos / Board_size, pos % Board_size, turn, 0);
	if (turn == 0)
		White_num++;
	else
		Black_num++;
	int result = quality_status(turn);
	//board_state[pos/Board_size][pos%Board_size] = 4;
	undo(turn, 0);
	return result;
}

int quality_of_pos2(pos,turn){//first step is taken, turn is white
	int a,b,temp_pos, temp_qua;// the same as these in find_best3
	board_state[pos/Board_size][pos%Board_size] = turn; //change the board_state
	flip(pos/Board_size, pos%Board_size, turn,0); //flip the board
	if(turn==0) White_num++;
	else Black_num++;//change the number
	//first step is taken, turn is white
	//如果有时间的话，建议把这三步放在同一个函数里，参数为pos_x, pos_y, turn, ifDraw


	turn = 1 - turn;// to take the second step, turn is black
	if (FindAvailable(turn, 1)) {
		temp_pos = find_best2(turn,0);
		board_state[temp_pos/Board_size][temp_pos%Board_size] = turn;
		flip(temp_pos/Board_size, temp_pos%Board_size, turn,0);
		if(turn==0) White_num++;
		else Black_num++;
		//take the second step, black;

		FindAvailable(1-turn, 1);
		temp_qua = find_best2(1-turn,1);
		//find the best result when the third step, white, is taken;

		undo(turn,0);
		//undo the second step, black
	}
	turn = 1 - turn;//turn from black to white
	undo(turn);// undo the first step, white
	return temp_qua;
}

int quality_status( turn) {
	int result = 0;
	int temp_state = 0;
	int empty_count = 0;
	int temp_x = -1;
	int temp_y = -1;
	int turn_count = 0;
	int turn_bar_count = 0;
	int a, b, c, d;
	int index1[6] = { 0, 1, 8, 9, 1, 1 };
	int index2[6] = { 7, 6, 14, 15, -1, 1 };
	int index3[6] = { 56, 48, 49, 57, 1, -1 };
	int index4[6] = { 63, 54, 55, 62, -1, -1 };
	int pos;
	for (a = 0; a < Board_size; a++) {
		for (b = 0; b < Board_size; b++) {
			temp_state = board_state[a][b];
			pos = a * Board_size + b;
			if (temp_state == 2 || temp_state == 4)
				continue;
			empty_count = 0;
			for (c = -1; c < 1; c++) {
				for (d = -1; d < 1; d++) {
					temp_x = a + c;
					temp_y = b + d;
					if (temp_x >= 0 && temp_x < Board_size && temp_y >= 0
							&& temp_y < Board_size
							&& board_state[temp_x][temp_y] == 0) {
						empty_count++;
					}
				}
			}
			if (temp_state == turn) {
				turn_count++;
				result += EVAL_SCORES[pos];
				result -= empty_count * LIBERTY_SCORE;
			} else {
				turn_bar_count++;
				result -= EVAL_SCORES[pos];
				result += empty_count * LIBERTY_SCORE;
			}
			if (turn_count == 0) {
				return -INFINITE;
			}
			if (turn_bar_count == 0) {
				return INFINITE;
			}
			if (turn_count + turn_bar_count == Board_size * Board_size) {
				if (turn_count > turn_bar_count) {
					return INFINITE;
				} else {
					if (turn_bar_count > turn_count) {
						return -INFINITE;
					}
				}
			}

		}
	}

	result += bonus_of_state(index1, turn);
	result += bonus_of_state(index2, turn);
	result += bonus_of_state(index3, turn);
	result += bonus_of_state(index4, turn);

	return result;
}
int bonus_of_state(int *t, int turn) {
	int corner = t[0];
	int corner_state = board_state[corner];
	int cur_state;
	int temp_state;
	int result = 0;
	int temp_pos;
	int a;
	if (corner_state != 2 && corner_state != 4) {
		for (a = 1; a <= 3; ++a) {
			temp_state = board_state[t[a]];
			if (temp_state == 4 || temp_state == 2)
				continue;
			if (temp_state == turn)
				result -= EVAL_SCORES[t[a]];
			else
				result += EVAL_SCORES[t[a]];
		}
		temp_pos = t[0];
		for (a = 0; a < Board_size - 2; a++) {
			temp_pos += t[4];
			temp_state = board_state[temp_pos];
			if (temp_state != corner_state)
				break;
			if (corner_state == turn)
				result += BONUS_SCORE;
			else
				result -= BONUS_SCORE;
		}
		temp_pos = t[0];
		for (a = 0; a < Board_size - 2; a++) {
			temp_pos += t[5] * Board_size;
			temp_state = board_state[temp_pos];
			if (corner_state != temp_state)
				break;
			if (corner_state == turn)
				result += BONUS_SCORE;
			else
				result -= BONUS_SCORE;
		}
	}
	return result;
}
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

int FindAvailable(int turn, int CheckOrFind) { // 找出可以落子的位置，画出需调用DrawAvailable() //返回值1为有子可下，返回值为0为无子可下
	//如果只是查询是否有子可以下CheckOrFind=0(原IsAvailable的功能)，如果要在棋盘上画上可下的位置CheckOrFind=1
	if (CheckOrFind)
		EraseLittleChess();
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
					if (CheckOrFind)
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
					if (CheckOrFind)
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
					if (CheckOrFind)
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
					if (CheckOrFind)
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
					if (CheckOrFind)
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
					if (CheckOrFind)
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
					if (CheckOrFind)
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
					if (CheckOrFind)
						board_state[x_pos][y_pos] = 4;
					continue;
				}
			}
	if (CheckOrFind)
		DrawAvailable(turn);
	return result;
}

// 估计在main函数之中，每一次下子完都会调用这个函数，来判断是否有一方获得了胜利（我猜测的）
int check_win() { //0白赢，1黑赢，2平局,-1没有人赢,
	if (Black_num == 0)//黑子数为0
	{
		//DrawChess(0,0,0);
		return 0;
	}

	if (White_num == 0)//白子数为0
	{
		//DrawChess(1,0,1);
		return 1;
	}

	if ((FindAvailable(0, 0) == 0 && FindAvailable(1, 0) == 0)//双方都无子可下
			|| ((Black_num + White_num) == Board_size * Board_size)) {
		//DrawChess(2,0,1);
		if (Black_num > White_num)
			return 1;
		if (Black_num < White_num)
			return 0;
		if (Black_num == White_num)
			return 2;
	}
	return -1;
}
void flip(int x_pos, int y_pos, int turn, int ifDraw) { //仅实现翻转功能
	int i, j;
	int flip_flag, flip_flag2, bk_flag;
	int *posS = (int*) malloc(sizeof(int) * 25);
	posS[0] = 1;
	//depth = 0;
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
			if (ifDraw)
				DrawChess(i, y_pos, turn);//画与turn的颜色覆盖
			lpush(posS, i * Board_size + y_pos);
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
			if (ifDraw)
				DrawChess(i, y_pos, turn);//画与turn的颜色覆盖
			lpush(posS, i * Board_size + y_pos);
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
			if (ifDraw)
				DrawChess(x_pos, i, turn);
			lpush(posS, x_pos * Board_size + i);
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
			if (ifDraw)
				DrawChess(x_pos, i, turn);
			lpush(posS, x_pos * Board_size + i);
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
			if (ifDraw)
				DrawChess(i, j, turn);
			lpush(posS, i * Board_size + j);
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
			if (ifDraw)
				DrawChess(i, j, turn);
			lpush(posS, i * Board_size + j);
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
			if (ifDraw)
				DrawChess(i, j, turn);
			lpush(posS, i * Board_size + j);
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
			if (ifDraw)
				DrawChess(i, j, turn);
			lpush(posS, i * Board_size + j);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}
	lpush(posS, x_pos * Board_size + y_pos);
	push(posS);
}
void undo(int turn, int ifDraw) {
	int *p = pop();
	if (p == NULL)
		return;
	int pos;
	pos = lpop(p);

	board_state[pos / Board_size][pos % Board_size] = 4;
	if (ifDraw)
		DrawChess(pos / Board_size, pos % Board_size, 2);
	if (turn == 1) {
		Black_num--;
	} else {
		White_num--;
	}
	while (1) {
		pos = lpop(p);
		if (pos == -1)
			break;
		board_state[pos / Board_size][pos % Board_size] = 1 - turn;
		if (ifDraw)
			DrawChess(pos / Board_size, pos % Board_size, 1 - turn);
		if (turn == 1) {
			Black_num--;
			White_num++;
		} else {
			Black_num++;
			White_num--;
		}
	}
	free(p);
}
void Draw_win( win_status) {
	//DrawChess(3,0,1);
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
	int Mode = 0;
	static XPs2 Ps2Inst;
	XPs2_Config *ConfigPtr;
	u32 StatusReg;
	u32 BytesReceived;
	u8 RxBuffer; // RxBuffer这个变量比较重要，在主循环逻辑部分我们需要通过判断这个值来判断到底键盘输入了哪个按键
	int count = 0, count1 = 0;
	int win_status = -1;
	int i, j;
	int x_cur = 5, y_cur = 5; // 这个是定义了一个x，y的初始坐标吧
	int turn = 1; // 定义了一个初始的棋子颜色（难道是下子时移动的光标？）
	int regret = 0;
	int a = 0;
	p1[0] = NULL;
	p1[1] = NULL;
	depth = 0;
	myflag = 0;

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
	DrawMenu();
	DrawStatus(0);

	while (1) { // 进入了主循环之中，游戏开始！
		do {
			dip_check = XGpio_DiscreteRead(&dip, 1);
			StatusReg = XPs2_GetStatus(&Ps2Inst);
		} while ((StatusReg & XPS2_STATUS_RX_FULL) == 0);
		BytesReceived = XPs2_Recv(&Ps2Inst, &RxBuffer, 1);
		count = (count + 1) % 3; // 这个count的功能是？防抖吗？似乎对主程序没有逻辑上的影响
		if (count == 1) {
			if (RxBuffer == 0x16) //按1键进入C1模式，简单人机对战
				Mode = 1;
			if (RxBuffer == 0x1E) //按2键进入C2模式，一般难度人机对战
				Mode = 2;
			if (RxBuffer == 0x26) //按3键进入C3模式，高难度人机对战
				Mode = 3;
			if (RxBuffer == 0x25) //按4键进入，人人对战模式
				Mode = 4;

			if (Mode != 0) { //若选择了模式，进入游戏
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
				FindAvailable(turn, 1);
				DrawAvailable(turn);
				DrawChess(x_cur, y_cur, 3); // 画出初始的旗子所在处的颜色，其中3这个参数代表的颜色应该是移动的光标的颜色
				DrawScore(2, 0); // 初始化显示出白棋的分数
				DrawScore(2, 1); // 初始化显示出黑棋的分数
				DrawStatus(Mode);
				//int boardStatus = 4;
				//DrawStatus(boardStatus);
				while (1) {
					do {
						dip_check = XGpio_DiscreteRead(&dip, 1);
						StatusReg = XPs2_GetStatus(&Ps2Inst);
					} while ((StatusReg & XPS2_STATUS_RX_FULL) == 0);
					BytesReceived = XPs2_Recv(&Ps2Inst, &RxBuffer, 1);
					count1 = (count1 + 1) % 3;
					if (count1 == 1) {

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
							if (win_status != -1) {
								Mode = 0;
								win_status = -1;
								x_cur = 5, y_cur = 5; // 这个是定义了一个x，y的初始坐标吧
								turn = 1; // 定义了一个初始的棋子颜色（难道是下子时移动的光标？）
								regret = 0;
								int a = 0;
								p1[0] = NULL;
								p1[1] = NULL;
								depth = 0;
								myflag = 0;

								xil_printf("-- Game Starts! --\r\n"); // 这个应该是在某一个地方输出一段话，方便debug
								for (i = 0; i < 8; i++) {
									for (j = 0; j < 8; j++) {
										board_state[i][j] = 2; // 将board_state[][]这个二维数组里面的所有旗子初始类型都定义为2，也就是无子
									}
								}
								DrawBoard();
								DrawMenu();
								DrawStatus(0);
								break;
							}
							// return 0;
							if (board_state[x_cur][y_cur] == 4) {
								regret = 0;
								EraseLittleChess();
								DrawChess(x_cur, y_cur, turn);
								if (turn == 0) {
									White_num++;
								} else {
									Black_num++;
								}
								board_state[x_cur][y_cur] = turn;
								flip(x_cur, y_cur, turn, 1);
								win_status = check_win();
								if (win_status != -1) { // 若有一方胜利了，本轮游戏结束
									DrawScore(White_num, 0);
									DrawScore(Black_num, 1);

									Draw_win(win_status);//画出显示胜利的棋盘形状
									//						xil_printf("\r\nPlayer %x wins!\r\n", turn + 1);
									//						return 0;

								} else {
									if (Mode == 4) { //若为人人对战模式，换一方下

										turn = 1 - turn; // 轮换黑白次序

										if (!FindAvailable(turn, 1)) {//一方五子可下时换另一方下
											turn = 1 - turn;
											FindAvailable(turn, 1);
										}
										DrawAvailable(turn);
									} else {
										//若为人机对战模式，由电脑下

										while (1) {

											if (!FindAvailable(0, 1)) {//若机器无子可下
												turn = 1;
												break;//由于check_win显示未分胜负，所以人一定有子可下，跳出循环，交给键盘
											}
											int temp_result;
											if (Mode == 1)
												temp_result = find_best1(0);
											else if (Mode == 2)
												temp_result = find_best2(0, 0);
											else if (Mode == 3)
												temp_result = find_best3(0);

											x_cur = temp_result / 8;
											y_cur = temp_result % 8;
											board_state[x_cur][y_cur] = 0;
											DrawChess(x_cur, y_cur, 0);
											White_num++;
											flip(x_cur, y_cur, 0, 1);
											win_status = check_win();
											//break;

											if (win_status != -1) { // 若有一方胜利了，本轮游戏结束
												Draw_win(win_status);//画出显示胜利的棋盘形状
												//						xil_printf("\r\nPlayer %x wins!\r\n", turn + 1);
												//						return 0;

											} else {
												if (FindAvailable(1, 1)) {
													break; //若人有子可下，跳出循环，交给键盘
												}
											}

										}
									}
								}
							}
							DrawScore(White_num, 0);
							DrawScore(Black_num, 1);

						}

						if (RxBuffer == 0x2D && win_status == -1 && Mode == 4) {

							turn = 1 - turn;
							undo(turn, 1);
							FindAvailable(turn, 1);
							DrawScore(White_num, 0); // 初始化显示出白棋的分数
							DrawScore(Black_num, 1); // 初始化显示出黑棋的分数
							regret++;
						}
						if (RxBuffer == 0x2D && win_status == -1 && Mode != 4) {
							undo(0, 1);
							undo(1, 1);
							FindAvailable(1, 1);
							DrawScore(White_num, 0); // 初始化显示出白棋的分数
							DrawScore(Black_num, 1); // 初始化显示出黑棋的分数
							regret++;
						}

						if (RxBuffer == 0x76) { // 0x76这个值对应的应该是restart，对应的是esc键
							// return 0;
							Mode = 0;
							win_status = -1;
							x_cur = 5, y_cur = 5; // 这个是定义了一个x，y的初始坐标吧
							turn = 1; // 定义了一个初始的棋子颜色（难道是下子时移动的光标？）
							regret = 0;
							int a = 0;
							p1[0] = NULL;
							p1[1] = NULL;
							depth = 0;
							myflag = 0;

							xil_printf("-- Game Starts! --\r\n"); // 这个应该是在某一个地方输出一段话，方便debug
							for (i = 0; i < 8; i++) {
								for (j = 0; j < 8; j++) {
									board_state[i][j] = 2; // 将board_state[][]这个二维数组里面的所有旗子初始类型都定义为2，也就是无子
								}
							}
							DrawBoard();
							DrawMenu();
							DrawStatus(0);
							break;
						}
					}

				}
			}

		}
	}
}
