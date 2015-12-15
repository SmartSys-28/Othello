# Othello

## Part 1: 新增功能

### 一，介绍
* `void DrawScore(int score, int turn)`, 此函数可以用来绘制黑白双方的实时得分。
* `void DrawMenu()`, 此函数可以用来绘制出右侧的最基本菜单。
* [MenuDraw.c](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/SDK/SDK_Export/lab_gobang_2/src/MenuDraw.c) 文件中还提供了其他的一些Draw函数，但一般不需要你们来调用，你们只要去使用最顶层的那两个函数即可。

### 二，使用方法
* 要使用这两个函数，首先必须保证你使用的是最新版的 [user_logic.v](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/pcores/vga_ip_v1_00_a/hdl/verilog/user_logic.v)，并且已经编译产生了最新的bit文件。然后，将 [MenuDraw.h](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/SDK/SDK_Export/lab_gobang_2/src/MenuDraw.h) 和 [MenuDraw.c](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/SDK/SDK_Export/lab_gobang_2/src/MenuDraw.c) 放入你的SDK源码工程中，然后在你的主函数所在c文件的最上部加入
```c
import "MenuDraw.h"
```

* 当你在改变`Whit_num`和`Black_num`的取值之时，都应该调用`DrawScore()`函数。正如我在本个项目的 [lab_gobang_2.c](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/SDK/SDK_Export/lab_gobang_2/src/lab_gobang_2.c) 文件中所展示的一样，在键入enter键后：
```c
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
```
在最下方调用了两次`DrawScore()`，绘制了黑白双方的分数。

* `DrawMenu()` 函数用于初始化右边的整个menu，在`DrawMenu()`函数中，我们已经调用了
```c
DrawScore(2, 0); // 初始化显示出白棋的分数
DrawScore(2, 1); // 初始化显示出黑棋的分数
```
这两句，所以正如正如我在本个项目的 [lab_gobang_2.c](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/SDK/SDK_Export/lab_gobang_2/src/lab_gobang_2.c) 文件中所展示的一样：
```c
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
```
在初始化整个界面的最后，加上了`DrawMenu()`初始化右边Menu的操作，便可以了。

## Part 2: BUG修正

* 重写了 [lab_gobang_2.c](https://github.com/SmartSys-28/Othello/blob/master/lab_gobang_2/SDK/SDK_Export/lab_gobang_2/src/lab_gobang_2.c) 中的EraseCursor()函数，如下：
```c
void EraseCursor(int x_pos, int y_pos, int turn) {
	if (turn == 1 && board_state[x_pos][y_pos] == 4)
		DrawChess(x_pos, y_pos, 5);
	else
		DrawChess(x_pos, y_pos, board_state[x_pos][y_pos]);
}
```
这样在光标移动过之后，不会把黑色小圆点还原成白色小圆点。<br>
（之前的话，因为在Draw黑色小圆点的时候，`board_state[][]`数组在那一点的取值仍为4，在`EraseCursor()`经过后会被还原成白色小圆点。）
