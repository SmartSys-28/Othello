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
//int *p1[2];
int stack[50];
int depth;
const int INFINITE = 99999;
//void push(int *x) {
//	if(depth % 2 == 0) {
//		if(p1[0] != NULL) free(p1[0]);
//		p1[0] = x;
//	}
//	else if(depth % 2 == 1) {
//		if(p1[1] != NULL) free(p1[1]);
//		p1[1] = x;
//	}
//	depth = (depth + 1) % 2;
//	p1[depth] = x;
//	depth++;
//}

const int EVAL_SCORES[64] = { 500, -150, 30, 10, 10, 30, -150, 500, -150, -250,
		0, 0, 0, 0, -250, -150, 30, 0, 1, 2, 2, 1, 0, 30, 10, 0, 2, 16, 16, 2,
		0, 10, 10, 0, 2, 16, 16, 2, 0, 10, 30, 0, 1, 2, 2, 1, 0, 30, -150,
		-250, 0, 0, 0, 0, -250, -150, 500, -150, 30, 10, 10, 30, -150, 500 };

//int* pop() {
//	depth = 1 - depth;
//	return p1[depth];
//}

void lpush(int x) {
	if (depth >= 50)
		return;
	stack[depth] = x;
	depth++;
}

int lpop() {
	if (depth <= 0)
		return -1;
	depth--;
	return stack[depth];
}
int find_best( turn) { //��������EVAL_SCORES��ֵѡ��ѡ�������Ǹ�
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

// ������ĳ����У�����Ҫ����x_cor, y_cor��x_pos, y_pos������
// ���У�x_pos��y_pos��board�ϵĲ������Ƿǳ�������һ��int�����������������ڵ�ĳһ��ĳһ��
// ��x_cor��y_cor����x_pos��y_pos����λ����ת��������һ��data�����Ǹ�VGA��ͼʹ�õ�

// ������������������������̵��ߵ�
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

// �����������������ÿһ�����ӵģ������ӵĻ���������̱������ɫ�����Ǳ���ƶ��Ĺ����ɫ����
void DrawChess(int x_pos, int y_pos, int turn) {
	int x_cor, y_cor, type, vga_input;
	type = turn;
	x_cor = x_pos << 24;
	y_cor = y_pos << 16;
	vga_input = x_cor + y_cor + type;
	VGA_IP_mWriteReg(XPAR_VGA_IP_0_BASEADDR, 0, vga_input);
}

// ��������Ļ�����һ��ר��Ĩȥ���ĺ�������Ϊ���Ҫ�������ϲ�ͣ�ƶ������ƶ�����һ��λ��ʱ��Ҫ����һ��λ�õĹ�껹ԭ��������ɫ��
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
	for (x_pos = 0; x_pos < Board_size; x_pos++) //0��1��2����3���4������
		for (y_pos = 0; y_pos < Board_size; y_pos++) { //board_stateֻȡ0 1 2��4��ʾ������
			if (board_state[x_pos][y_pos] == 4 && turn == 0)
				DrawChess(x_pos, y_pos, 4);
			else if (board_state[x_pos][y_pos] == 4 && turn == 1)
				DrawChess(x_pos, y_pos, 5);
		}
}

int FindAvailable(int turn, int CheckOrFind) { // �ҳ��������ӵ�λ�ã����������DrawAvailable() //����ֵ1Ϊ���ӿ��£�����ֵΪ0Ϊ���ӿ���
	//���ֻ�ǲ�ѯ�Ƿ����ӿ�����CheckOrFind=0(ԭIsAvailable�Ĺ���)�����Ҫ�������ϻ��Ͽ��µ�λ��CheckOrFind=1
	if (CheckOrFind)
		EraseLittleChess();
	int i, j;
	int flip_flag, flip_flag2;
	int result = 0;
	int x_pos, y_pos;
	for (x_pos = 0; x_pos < Board_size; x_pos++) //0��1��2����3���4������
		for (y_pos = 0; y_pos < Board_size; y_pos++) //board_stateֻȡ0 1 2��4��ʾ������
			if (board_state[x_pos][y_pos] == 2) {
				flip_flag = -1;
				if ((x_pos + 1 < Board_size) && (board_state[x_pos + 1][y_pos]
						== (1 - turn)))
					for (i = x_pos + 1; i < Board_size; i++) { // ����ˮƽ��������жϣ������ж��ٸ�ͬ��ɫ��������
						if (board_state[i][y_pos] == 2 || board_state[i][y_pos]
								== 4)
							break;
						else if (board_state[i][y_pos] == turn) { //�ж�����������Ƿ��п��Է�ת������
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
					for (i = x_pos - 1; i >= 0; i--) { // ����ˮƽ��������жϣ������ж��ٸ�ͬ��ɫ��������
						if (board_state[i][y_pos] == 2 || board_state[i][y_pos]
								== 4)
							break;
						else if (board_state[i][y_pos] == turn) { //�ж�����������Ƿ��п��Է�ת������
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
					for (i = y_pos + 1; i < Board_size; i++) { // ������ֱ��������жϣ������ж��ٸ�ͬ��ɫ��������
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
					for (i = y_pos - 1; i >= 0; i--) { // ������ֱ��������жϣ������ж��ٸ�ͬ��ɫ��������
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
							< Board_size; i++, j++) { // ��������������жϣ������ж��ٸ�ͬ��ɫ��������
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
					for (i = x_pos - 1, j = y_pos - 1; i >= 0 && j >= 0; i--, j--) { // �����Ϸ�������жϣ������ж��ٸ�ͬ��ɫ��������
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
					for (i = x_pos + 1, j = y_pos - 1; i < Board_size && j >= 0; i++, j--) { // �����Ϸ�������жϣ������ж��ٸ�ͬ��ɫ��������
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
					for (i = x_pos - 1, j = y_pos + 1; i >= 0 && j < Board_size; i--, j++) { // ��������������жϣ������ж��ٸ�ͬ��ɫ��������
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

// ������main����֮�У�ÿһ�������궼�����������������ж��Ƿ���һ�������ʤ�����Ҳ²�ģ�
int check_win() { //0��Ӯ��1��Ӯ��2ƽ��,-1û����Ӯ,
	if (Black_num == 0)//������Ϊ0
		return 0;
	if (White_num == 0)//������Ϊ0
		return 1;
	if ((FindAvailable(0, 0) == 0 && FindAvailable(1, 0) == 0)//˫�������ӿ���
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
void flip(int x_pos, int y_pos, int turn) { //��ʵ�ַ�ת����
	int i, j;
	int flip_flag, flip_flag2, bk_flag;
	//int *posS = (int*) malloc(sizeof(int) * 50);
	//posS[0] = 1;
	depth = 0;
	flip_flag = -1;
	if ((x_pos + 1 < Board_size) && (board_state[x_pos + 1][y_pos]
			== (1 - turn)))
		for (i = x_pos + 1; i < Board_size; i++) { // ����ˮƽ��������жϣ������ж��ٸ�ͬ��ɫ��������
			if (board_state[i][y_pos] == 2 || board_state[i][y_pos] == 4)
				break;
			else if (board_state[i][y_pos] == turn) { //�ж�����������Ƿ��п��Է�ת������
				flip_flag = i;
				break;
			}
		}

	//��ת
	if (flip_flag != -1)
		for (i = x_pos + 1; i < flip_flag; i++) {
			board_state[i][y_pos] = turn;
			DrawChess(i, y_pos, turn);//����turn����ɫ����
			lpush(i * Board_size + y_pos);
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
		for (i = x_pos - 1; i >= 0; i--) { // ����ˮƽ��������жϣ������ж��ٸ�ͬ��ɫ��������
			if (board_state[i][y_pos] == 2 || board_state[i][y_pos] == 4)
				break;
			else if (board_state[i][y_pos] == turn) { //�ж�����������Ƿ��п��Է�ת������
				flip_flag = i;
				break;
			}
		}

	if (flip_flag != -1)
		for (i = x_pos - 1; i > flip_flag; i--) {
			board_state[i][y_pos] = turn;
			DrawChess(i, y_pos, turn);//����turn����ɫ����
			lpush(i * Board_size + y_pos);
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
		for (i = y_pos + 1; i < Board_size; i++) { // ������ֱ��������жϣ������ж��ٸ�ͬ��ɫ��������
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
			lpush(x_pos * Board_size + i);
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
		for (i = y_pos - 1; i >= 0; i--) { // ������ֱ��������жϣ������ж��ٸ�ͬ��ɫ��������
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
			lpush(x_pos * Board_size + i);
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
		for (i = x_pos + 1, j = y_pos + 1; i < Board_size && j < Board_size; i++, j++) { // ��������������жϣ������ж��ٸ�ͬ��ɫ��������
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
			lpush(i * Board_size + j);
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
		for (i = x_pos - 1, j = y_pos - 1; i >= 0 && j >= 0; i--, j--) { // �����Ϸ�������жϣ������ж��ٸ�ͬ��ɫ��������
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
			lpush(i * Board_size + j);
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
		for (i = x_pos + 1, j = y_pos - 1; i < Board_size && j >= 0; i++, j--) { // �����Ϸ�������жϣ������ж��ٸ�ͬ��ɫ��������
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
			lpush(i * Board_size + j);
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
		for (i = x_pos - 1, j = y_pos + 1; i >= 0 && j < Board_size; i--, j++) { // ��������������жϣ������ж��ٸ�ͬ��ɫ��������
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
			lpush(i * Board_size + j);
			if (turn == 0) {
				Black_num--;
				White_num++;
			} else {
				Black_num++;
				White_num--;
			}
		}
	lpush(x_pos * Board_size + y_pos);
	//push(posS);
}
void undo(int turn) {
	int pos;
	pos = lpop();
	board_state[pos / Board_size][pos % Board_size] = 4;
	DrawChess(pos / Board_size, pos % Board_size, 2);
	if (turn == 1) {
		Black_num--;
	} else {
		White_num--;
	}
	while (1) {
		pos = lpop();
		if (pos == -1)
			break;
		board_state[pos / Board_size][pos % Board_size] = 1 - turn;
		DrawChess(pos / Board_size, pos % Board_size, 1 - turn);
		if (turn == 1) {
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
		color = 1 - color; //�����ƽ�ַ�ת��ɫ
	DrawChess(1, 3, color);
	DrawChess(2, 5, color);
	DrawChess(3, 3, color);
	DrawChess(4, 5, color);
	DrawChess(5, 3, color);
}

// ���������������������Ӧ�ö�����������
int main(void) {
	XGpio dip; // dip�Ƕ����һ���������ͣ�
	int dip_check;
	int Mode = 0;
	static XPs2 Ps2Inst;
	XPs2_Config *ConfigPtr;
	u32 StatusReg;
	u32 BytesReceived;
	u8 RxBuffer; // RxBuffer��������Ƚ���Ҫ������ѭ���߼�����������Ҫͨ���ж����ֵ���жϵ��׼����������ĸ�����
	int count = 0, count1 = 0;
	int win_status = -1;
	int i, j;
	int x_cur = 5, y_cur = 5; // ����Ƕ�����һ��x��y�ĳ�ʼ�����
	int turn = 1; // ������һ����ʼ��������ɫ���ѵ�������ʱ�ƶ��Ĺ�ꣿ��
	int regret = 0;
	depth = 0;

	xil_printf("-- Game Starts! --\r\n"); // ���Ӧ������ĳһ���ط����һ�λ�������debug

	XGpio_Initialize(&dip, XPAR_DIP_SWITCHES_8BITS_DEVICE_ID);
	XGpio_SetDataDirection(&dip, 1, 0xffffffff);

	ConfigPtr = XPs2_LookupConfig(XPAR_XPS_PS2_0_0_DEVICE_ID);
	XPs2_CfgInitialize(&Ps2Inst, ConfigPtr, ConfigPtr->BaseAddress); // ��һ��Ӧ����һЩ��԰����Լ����̵ĳ�ʼ������

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board_state[i][j] = 2; // ��board_state[][]�����ά����������������ӳ�ʼ���Ͷ�����Ϊ2��Ҳ��������
		}
	}
	DrawBoard();
	DrawMenu();
	DrawStatus(0);

	while (1) { // ��������ѭ��֮�У���Ϸ��ʼ��
		do {
			dip_check = XGpio_DiscreteRead(&dip, 1);
			StatusReg = XPs2_GetStatus(&Ps2Inst);
		} while ((StatusReg & XPS2_STATUS_RX_FULL) == 0);
		BytesReceived = XPs2_Recv(&Ps2Inst, &RxBuffer, 1);
		count = (count + 1) % 3; // ���count�Ĺ����ǣ��������ƺ���������û���߼��ϵ�Ӱ��
		if (count == 1) {
			if (RxBuffer == 0x16) //��1������C1ģʽ�����˻���ս
				Mode = 1;
			if (RxBuffer == 0x1E) //��2������C2ģʽ��һ���Ѷ��˻���ս
				Mode = 2;
			if (RxBuffer == 0x26) //��3������C3ģʽ�����Ѷ��˻���ս
				Mode = 3;
			if (RxBuffer == 0x25) //��4�����룬���˶�սģʽ
				Mode = 4;

			if (Mode != 0) { //��ѡ����ģʽ��������Ϸ
				DrawChess(3, 3, 1); // ���ݺڰ���Ĺ��򣬳�ʼ�����������Ӻ͸���board_state[][]����
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
				DrawChess(x_cur, y_cur, 3); // ������ʼ���������ڴ�����ɫ������3��������������ɫӦ�����ƶ��Ĺ�����ɫ
				DrawScore(2, 0); // ��ʼ����ʾ������ķ���
				DrawScore(2, 1); // ��ʼ����ʾ������ķ���
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

						if (RxBuffer == 0x1D && win_status == -1) { // 0x1D���ֵӦ�ö�Ӧ���������ƶ���Ҳ����W��
							EraseCursor(x_cur, y_cur, turn);
							if (y_cur == 0)
								y_cur = 7;
							else
								y_cur--;
							DrawChess(x_cur, y_cur, 3);
						}
						if (RxBuffer == 0x1B && win_status == -1) { // 0x1B���ֵӦ�ö�Ӧ���������ƶ���Ҳ����S��
							EraseCursor(x_cur, y_cur, turn);
							if (y_cur == 7)
								y_cur = 0;
							else
								y_cur++;
							DrawChess(x_cur, y_cur, 3);
						}
						if (RxBuffer == 0x1C && win_status == -1) { // 0x1C���ֵ��Ӧ��Ӧ���������ƶ���Ҳ����A��
							EraseCursor(x_cur, y_cur, turn);
							if (x_cur == 0)
								x_cur = 7;
							else
								x_cur--;
							DrawChess(x_cur, y_cur, 3);
						}
						if (RxBuffer == 0x23 && win_status == -1) { // 0x23���ֵ��Ӧ��Ӧ���������ƶ���Ҳ����D��
							EraseCursor(x_cur, y_cur, turn);
							if (x_cur == 7)
								x_cur = 0;
							else
								x_cur++;
							DrawChess(x_cur, y_cur, 3);
						}

						if (RxBuffer == 0x5A) { // 0x5A���ֵ��Ӧ��Ӧ����ȷ������Ҳ����enter��
							if (win_status != -1)
								return 0;
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
								flip(x_cur, y_cur, turn);
								win_status = check_win();
								if (win_status != -1) { // ����һ��ʤ���ˣ�������Ϸ����
									DrawScore(White_num, 0);
									DrawScore(Black_num, 1);
									Draw_win(win_status);//������ʾʤ����������״
									//						xil_printf("\r\nPlayer %x wins!\r\n", turn + 1);
									//						return 0;

								} else {
									if (Mode == 4) { //��Ϊ���˶�սģʽ����һ����
										turn = 1 - turn; // �ֻ��ڰ״���
										if (!FindAvailable(turn, 1)) {//һ�����ӿ���ʱ����һ����
											turn = 1 - turn;
											FindAvailable(turn, 1);
										}
										DrawAvailable(turn);
									} else
										//��Ϊ�˻���սģʽ���ɵ�����
										while (1) {
											turn = 1 - turn; // �ֻ��ڰ״���
											//human = 1 - human;//�ɻ�����
											//EraseLittleChess();
											if (!FindAvailable(turn, 1)) {//���������ӿ���
												turn = 1 - turn; // �ֻ��ڰ״���
												//human = 1 - human;//������
												break;//����check_win��ʾδ��ʤ����������һ�����ӿ��£�����ѭ������������
											}
											int temp_result;
											temp_result = find_best(turn);
											x_cur = temp_result / 8;
											y_cur = temp_result % 8;
											board_state[x_cur][y_cur] = turn;
											DrawChess(x_cur, y_cur, turn);
											if (turn == 0) {
												White_num++;
											} else {
												Black_num++;
											}
											flip(x_cur, y_cur, turn);
											win_status = check_win();
											if (win_status != -1) { // ����һ��ʤ���ˣ�������Ϸ����
												Draw_win(win_status);//������ʾʤ����������״
												//						xil_printf("\r\nPlayer %x wins!\r\n", turn + 1);
												//						return 0;

											} else {
												turn = 1 - turn;//����һ��
												//human = 1 - human;//������
												//EraseLittleChess();
												if (FindAvailable(turn, 1))
													break; //�������ӿ��£�����ѭ������������
											}

										}
								}
							}
							DrawScore(White_num, 0);
							DrawScore(Black_num, 1);

						}

						if (RxBuffer == 0x2D && win_status == -1 && Mode == 4   // 0x76���ֵ��Ӧ��Ӧ���ǻ��壬��Ӧ����R��
								&& regret == 0) {

							turn = 1 - turn;
							undo(turn);
							FindAvailable(turn, 1);
							DrawScore(White_num, 0); // ��ʼ����ʾ������ķ���
							DrawScore(Black_num, 1); // ��ʼ����ʾ������ķ���
							regret = 1;
						}

						if (RxBuffer == 0x76) { // 0x76���ֵ��Ӧ��Ӧ����restart����Ӧ����esc��
							return 0;
						}
					}

				}
			}

		}
	}
}
