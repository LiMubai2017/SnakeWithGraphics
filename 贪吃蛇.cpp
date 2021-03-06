// 贪吃蛇.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include"view.h"
void CreatMap(void);
void control(void);
void print(void);
void InitFood(void);
void InitSnake(void);
void InitSnake2(void);
void InitGrass(void);
void InitBrick(void);
void move(void);
void move2(void);
void rank(void);
void AiControl(void);
int check(int,int);
void BrickFalling(char);
void BrickRolling(void);
void CheckBrick(void);
void NextChapter();
void Win();
void init();
void loadMapImage();
void LoadGame();
void SaveGame();
void pause();
int LoadOrNew();

extern void end(void);
extern void selectPrint(int);
extern void changeMap(int, int, int);
extern void pos(short, short);
extern void printElement(int, int, int, int);
extern void welcome(void);
extern void select(void);
extern void EndPro(int);

int main()
{
	printf("Name: ");
	scanf_s("%s", mname,10*sizeof(char));
	initgraph(1000, 600);
	//welcome();

	init();
}

void init()
{
	srand(time(NULL));
	memset(map, 0, sizeof(map));
	memset(update, 0, sizeof(update));
	level = 0;
	chapter = 1;
	endFlag = false;
	cleanScreen();
	score = 0;
	SnakeColor = BLUE;
	//第三关复原
	LEFT = 75;UP = 72;RIGHT = 77;DOWN = 80;

	Snake *current = head1;
	while (head1 != NULL) {
		current = head1;
		head1 = head1->next;
		free(current);
	}
	current = head2;
	while (head2 != NULL) {
		current = head1;
		head2 = head2->next;
		free(current);
	}

	select();
	if (level == 2)
		if (LoadOrNew() == 1) {
			loadMapImage();
			CreatMap();
			InitSnake();
			InitFood();
			control();
		}
		else
			LoadGame();
	else {
		loadMapImage();
		CreatMap();
		InitSnake();
		if (level == 4) InitSnake2();
		InitFood();
		control();
	}
}


void CreatMap(void)
{
	int i;
	for (i = 0;i <= col[level] - 2;i += 2) {
		printElement(BORDER,0, i, WHITE);
		changeMap(BORDER,0, i);
	}
	for (i = 1;i<lines[level] - 1;i++)
	{
		printElement(BORDER,i, 0,WHITE);
		printElement(BORDER,i, col[level] - 2,WHITE);
		changeMap(BORDER, i, 0);
		changeMap(BORDER, i, col[level] - 2);
	}
	for (i = 0;i <= col[level] - 2;i += 2) {
		printElement(BORDER, lines[level]-1, i, WHITE);
		changeMap(BORDER, lines[level]-1, i);
	}
	if (level == 2) {
		int x, y;
		switch (chapter) {
		case 1:
			for (int i = 0; i <= lines[level] / 2 + 8; i++) {
				x = i;y = col[level] - 12-2*i;
				printElement(BORDER, x, y, WHITE);
				changeMap(BORDER, x, y);
				x = lines[level]-1-i;y = 12+2 * i;
				printElement(BORDER, x, y, WHITE);
				changeMap(BORDER, x, y);
			}
			for (int i = 0; i <= lines[level] / 2-4 ; i++) {
				x = lines[level]/2-i;y =2+ 2 * i;
				printElement(BORDER, x, y, WHITE);
				changeMap(BORDER, x, y);
				x = lines[level]/2+i;y = col[level]-4- 2 * i;
				printElement(BORDER, x, y, WHITE);
				changeMap(BORDER, x, y);
			}
			break;
		case 2:
			for (i = 1;i <= barrierNum[chapter];i++) {
				x = 1 + rand() % (lines[level] - 3);
				y = 2 + rand() % (col[level] - 6);
				if (y % 2 != 0) y++;
				if (map[x][y] != BLANK) continue;
				printElement(BORDER, x, y, color[rand() % color[0] - 1]);
				changeMap(BORDER, x, y);
			}
			break;
		}

		/*for (i = 1;i <= barrierNum[chapter];i++) {
			x = 1 + rand() % (lines[level] - 3);
			y = 2 + rand() % (col[level] - 6);
			if (y % 2 != 0) y++;
			printElement(BORDER, x, y, WHITE);
			changeMap(BORDER, x, y);
		}*/
		switch (chapter) {
		case 1:
			for (i = 1;i <= boomNum[chapter];i++) {
				x = 1 + rand() % (lines[level] - 3);
				y = 2 + rand() % (col[level] - 6);
				if (y % 2 != 0) y++;
				changeMap(BOMB, x, y);
				printElement(BOMB, x, y, RED);
			}
			break;
		case 2:
			break;
		}
	}
	if (level == 3) {

		InitBrick();
	}
}
void rank(void)
{
	FILE *fin, *fout;
	int  i =0, scoreInRanking[5];
	char name[6][51];

	memset(scoreInRanking, 0, sizeof(scoreInRanking));
	i = 0;
	
	switch (level) {
	case 2:
		if(fopen_s(&fin,"ranking2.in", "r")!=0)
			return ;
		break;
	case 3:
		if (fopen_s(&fin, "ranking3.in", "r") != 0)
			return;
		break;
	}
	for (i = 0;i <= 4;i++) {
		fscanf_s(fin, "%s", name[i],50*sizeof(char));
		fscanf_s(fin, "%d", &scoreInRanking[i]);
	}
	if (fclose(fin) == EOF) exit(EXIT_FAILURE);
	for (i = 0;i <= 4;i++)
		if (totalScore>scoreInRanking[i]) {
			int j;
			for (j = 4;j>i;j--) {
				scoreInRanking[j] = scoreInRanking[j - 1];
				strcpy_s(name[j], name[j - 1]);
			}
			scoreInRanking[i] = totalScore;
			int length = strlen(mname);
			for (int z = 0;z <= length;z++)
				if (mname[z] == '\n')
					mname[z] = '\0';
			strcpy_s(name[i], mname);
			switch (level) {
			case 2:fopen_s(&fout,"ranking2.in", "w");break;
			case 3:fopen_s(&fout,"ranking3.in", "w");break;
			}
			for (i = 0;i <= 4;i++)
				fprintf(fout, "%s %d\n", name[i], scoreInRanking[i]);
			fclose(fout);
			break;
		}
	showRanking(name, scoreInRanking);
}

void InitFood(void)
{
	struct Food current;
	while ((NewColor = color[1+rand() % color[0]]) == SnakeColor) continue;
	while (map[current.x = 2 + rand() % (lines[level] - 2)][current.y = 4 + rand() % (col[level] - 8)] != BLANK|| current.y % 2 != 0) continue;
	printElement(BLANK, food.x, food.y, 0);
	map[food.x][food.y] = BLANK;
	printElement(FOOD, current.x, current.y, NewColor);
	map[current.x][current.y] = FOOD;
	food = current;
}


void InitBrick(void)
{
	BrickFlag = 1 + rand() % 4;
	brick[1].x = 1;
	while ((brick[1].y = 8 + rand() % (col[level] - 20)) % 2 != 0) continue;
	switch (BrickFlag) {
	case 1: {
		brick[2].x = brick[3].x = brick[4].x = 1;
		brick[2].y = brick[1].y - 2;
		brick[3].y = brick[1].y + 2;
		brick[4].y = brick[1].y + 4;
	}break;//长条 
	case 2: {
		brick[2].x = 2;brick[3].x = brick[4].x = 1;
		brick[2].y = brick[1].y;
		brick[3].y = brick[1].y + 2;
		brick[4].y = brick[1].y + 4;
	}break;//L条 
	case 3: {
		brick[2].x = brick[3].x = 1;brick[4].x = 2;
		brick[4].y = brick[1].y;
		brick[2].y = brick[1].y - 2;
		brick[3].y = brick[1].y + 2;
	}break;//凸 
	case 4: {
		brick[2].x = 1;brick[3].x = brick[4].x = 2;
		brick[4].y = brick[2].y = brick[1].y + 2;
		brick[3].y = brick[1].y;
	}break;//正方形 
	}

}
void InitGrass(void)
{

	int x, y, i, j;
	if (GlassFlag == 0 ) 
	if(!wisdomFlag){
		//第三关逻辑
		if (level == 2 && chapter == 3) {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\笑声.wav"), NULL, SND_FILENAME | SND_ASYNC);
			int temp = LEFT;LEFT = RIGHT;RIGHT = temp;
			temp = UP;UP = DOWN;DOWN = temp;
		}
		else
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\毒草产生.wav"), NULL, SND_FILENAME | SND_ASYNC);


		for (i = 1;i <= GrassNumber[chapter];i++) {
			x = 2 + rand() % (lines[level] - 2);
			y = 2 + rand() % (col[level] - 6);
			if (y % 2 != 0) y++;
			while (map[x][y] != BLANK)
				x = 2 + rand() % (lines[level] - 2);
			map[x][y] = POISON;
		}
		x = 2 + rand() % (lines[level] - 2);
		y = 2 + rand() % (col[level] - 6);
		if (y % 2 != 0) y++;
		while (map[x][y] != BLANK)
			x = 2 + rand() % (lines[level] - 2);
		map[x][y] = WISDOM;
		GlassFlag = 1;
	}
	else {
		//第三关逻辑
		if (level == 2 && chapter == 3) {
			int temp = LEFT;LEFT = RIGHT;RIGHT = temp;
			temp = UP;UP = DOWN;DOWN = temp;
		}
		PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\消失.wav"), NULL, SND_FILENAME | SND_ASYNC);

		for (i = 1;i <= lines[level] - 2;i++)
			for (j = 2;j <= col[level] - 4;j += 2)
				if (map[i][j] == POISON||map[i][j]==WISDOM) {
					printElement(BLANK,i, j,0);
					map[i][j] = BLANK;
				}
		GlassFlag = 0;

	}
}
void InitSnake(void)
{
	struct Snake *current;
	length = 2;
	direction = 'D';
	head1 = (struct Snake *)malloc(sizeof(struct Snake));
	if (level == 2) {
		switch (chapter) {
		case 1:
			head1->x = 3;head1->y = 6;
			break;
		case 2:
			head1->x = lines[level] / 2;head1->y = col[level] / 2;
			break;
		case 3:
			head1->x = lines[level] / 2;head1->y = col[level] / 2;
			break;	
		}
	} else {
		head1->x = lines[level] / 2;head1->y = col[level] / 2;
	}
	current = head1->next = (struct Snake *)malloc(sizeof(struct Snake));
	current->last = head1;
	current->x = head1->x-1;current->y = head1->y;current->next = NULL;
	current = head1;
	while (current != NULL) {
		printElement(SNAKE,current->x, current->y, BLUE);
		current = current->next;
	}
	food.x = food.y = 2;
}
void InitSnake2(void)
{
	struct Snake *current;
	direction2 = 'D';
	head2 = (struct Snake *)malloc(sizeof(struct Snake));
	head2->x = lines[level] / 2;head2->y = col[level] / 2 - 4;
	current = head2->next = (struct Snake *)malloc(sizeof(struct Snake));
	current->x = lines[level] / 2 - 1;current->y = col[level] / 2 - 4;
	current->next = NULL;current->last = head2;
	current = head2;
	while (current != NULL) {
		printElement(SNAKE,current->x,current->y,RED);
		current = current->next;
	}
}
int check(int a, int b)
{
	int new_x, new_y;
	new_x = head1->x + a;
	new_y = head1->y + b;
	if (map[new_x][new_y] == BLANK || map[new_x][new_y] == FOOD) return 1;
	return 0;

}
void CheckBrick(void)
{
	int i, j, CleanFlag;
	for (i = lines[level] - 2;i >= 1;i--) {
		CleanFlag = 1;
		for (j = 2;j <= col[level] - 4;j += 2)
			if (map[i][j] != STOPPED_BRICK) CleanFlag = 0;
		if (CleanFlag) break;
	}
	if (!CleanFlag) return;
	score += 10;
	int UpdateBrick[40][80];//1为从无到有，2为从有到无 
	memset(UpdateBrick, 0, sizeof(UpdateBrick));
	for (i = 2;i <= lines[level] - 2;i++)
		for (j = 2;j <= col[level] - 4;j += 2)
			if (map[i][j] == BLANK&&map[i - 1][j] == STOPPED_BRICK) UpdateBrick[i][j] = 1;
			else if (map[i][j] == STOPPED_BRICK&&map[i - 1][j] == BLANK) UpdateBrick[i][j] = 2;
			for (i = 2;i <= lines[level] - 2;i++)
				for (j = 2;j <= col[level] - 4;j += 2)
					if (UpdateBrick[i][j] == 1) {
						pos(i, j);
						printf("■");
						map[i][j] = STOPPED_BRICK;
					}
					else if (UpdateBrick[i][j] == 2) {
						pos(i, j);
						printf(" ");
						map[i][j] = BLANK;
					}
					CheckBrick();
}

void AImove(Point *end)
{
	int route[150];
	int index = 0;

	while (end->last != NULL) {
		int x = end->x, y = end->y;
		int lastX = end->last->x, lastY = end->last->y;

		if (x > lastX) route[++index] = 'D';
		else if (x < lastX) route[++index] = 'U';
		else if (y > lastY) route[++index] = 'R';
		else if (y < lastY) route[++index] = 'L';
		end = end->last;
	}
	for (int i = 1; i <= index / 2; i++) {
		int temp = route[i];
		route[i] = route[index + 1 - i];
		route[index + 1 - i] = temp;
	}
	for (int i = 1; i <= index; i++) {
		direction = route[i];
		move();
		Sleep(50);
	}

}

void AiControl(void)
{
	PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\bingbing.wav"), NULL, SND_FILENAME | SND_ASYNC);

	int mScore = score;

	while (score - mScore < 30 && score < targetscore[chapter]&&endFlag == 0) {
		//宽搜初始化
		Point *start ;
		num = 0;
		put(head1->x, head1->y, NULL);

		//宽搜进行
		while (num > 0) {
			Point *point = poll();
			if (map[point->x][point->y] == FOOD) {
				for (int i = 0; i < 40; i++)
					for (int j = 0; j < 80; j += 2)
						if (map[i][j] == OCCUPIED)
							map[i][j] = BLANK;
				AImove(point);
				break;
			}
			for (int i = 0; i < 4; i++) {
				int x = point->x + dx[i];
				int y = point->y + dy[i];
				if (x >= 0 && x < 40 && y >= 0 && y < 80 && (map[x][y] == BLANK || map[x][y] == FOOD)) {
					if(map[x][y]!=FOOD) map[x][y] = OCCUPIED;
					put(x, y, point);
				}

			}
		}

		//复原
		/*while (num > 0) {
			Point * current = poll();
			while (current != NULL) {
				Point * del = current;
				current = current->last;
				free(del);
			}
		}*/
	}

	PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\智慧草消失.wav"), NULL, SND_FILENAME | SND_ASYNC);
}




void control()
{
	while (_kbhit()) _getch();
	while (1) {
		//过关条件 
		if ((level == 2 ||level ==1)&& score >= targetscore[chapter]) {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\游戏完成.wav"), NULL, SND_FILENAME | SND_ASYNC);
			ending();
			NextChapter();
			break;
		}
		if (endFlag) {
			init();
			break;
		}
		int flag = 0, flag2 = 0;
		char ch;

		ch = 0;
		if (_kbhit()) ch = _getch();
		if (ch == VK_SPACE) pause();
		if (ch == FIRSTKEY) ch = _getch();
		if (ch == UP && direction != 'D'&&direction != 'U' && !flag) {
			direction = 'U';move();flag = 1;
		}
		if (ch == DOWN && direction != 'D'&&direction != 'U' && !flag) {
			direction = 'D';move();flag = 1;
		}
		if (ch == LEFT && direction != 'L'&&direction != 'R' && !flag) {
			direction = 'L';move();flag = 1;
		}
		if (ch == RIGHT && direction != 'L'&&direction != 'R' && !flag) {
			direction = 'R';move();flag = 1;
		}
		if (ch == 'c' || ch == 'C') AiControl();
		if (!flag) move();

		if (level == 3 ) {
			if(_kbhit()) ch = _getch();
			if (ch == VK_SPACE) pause();
			switch (ch) {
			case 'D':
			case 'd': {
				int BrickFlag = 0, i;
				for (i = 1;i <= 4;i++)
					if (brick[i].y >= col[level] - 4 || map[brick[i].x][brick[i].y + 2] == STOPPED_BRICK) BrickFlag = 1;
				if (!BrickFlag) BrickFalling('R');
			}break;
			case 'A':
			case 'a': {
				int BrickFlag = 0, i;
				for (i = 1;i <= 4;i++)
					if (brick[i].y <= 2 || map[brick[i].x][brick[i].y - 2] == STOPPED_BRICK) BrickFlag = 1;
				if (!BrickFlag) BrickFalling('L');
			}break;
			case 'W':
			case 'w': {
				int BrickFlag = 0, i;
				for (i = 1;i <= 4;i++)
					if (brick[i].y - 4<2) BrickFlag = 1;
				if (brick[i].y + 4>col[level] - 4) BrickFlag = 2;
				if (BrickFlag) {
					for (i = 1;i <= 4;i++) {
						map[brick[i].x][brick[i].y] = BLANK;
						pos(brick[i].x, brick[i].y);
						printf(" ");
					}
					if (BrickFlag == 1)
						for (i = 1;i <= 4;i++) brick[i].y += 4;
					if (BrickFlag == 2)
						for (i = 1;i <= 4;i++) brick[i].y -= 4;
					for (i = 1;i <= 4;i++) {
						map[brick[i].x][brick[i].y] = BRICK;
						printElement(BLANK, brick[i].x, brick[i].y, WHITE);
					}
				}
				BrickRolling();
			}break;
			}
		}
		if (level == 4) {
			if (_kbhit()) ch = _getch();
			if ((ch == 'w' || ch == 'W') && direction2 != 'D'&&direction2 != 'U' && !flag2) {
				direction2 = 'U';move2();flag2 = 1;
			}
			if ((ch == 's' || ch == 'S') && direction2 != 'D'&&direction2 != 'U' && !flag2) {
				direction2 = 'D';move2();flag2 = 1;
			}
			if ((ch == 'a' || ch == 'A') && direction2 != 'L'&&direction2 != 'R' && !flag2) {
				direction2 = 'L';move2();flag2 = 1;
			}
			if ((ch == 'd' || ch == 'D') && direction2 != 'L'&&direction2 != 'R' && !flag2) {
				direction2 = 'R';move2();flag2 = 1;
			}
			if (ch == VK_SPACE) pause();
		}
		if (level == 4 && !flag2) move2();
		/*if (GetAsyncKeyState(VK_SPACE))
			while (1) {
				Sleep(500);
				if (_kbhit()) {
					char ch;
					ch = _getch();
					if (ch == VK_SPACE) break;
					else
						if ((ch == 's' || ch == 'S') && level == 2) SaveGame();
				}
				if (GetAsyncKeyState(VK_SPACE)) break;
			}*/
		if (level == 3) BrickFalling('D');
		if (level == 1) Sleep(SleepTime[1]);
		if (level == 2) Sleep(SleepTime[2]);
		if (level == 3) Sleep(SleepTime[3]);
		if (level == 4) Sleep(SleepTime[4]);
	}

}

void pause()
{
	while (1) {
		Sleep(500);
		if (_kbhit()) {
			char ch;
			ch = _getch();
			if (ch == VK_SPACE) break;
			else
				if ((ch == 's' || ch == 'S') && level == 2) SaveGame();
		}
	}
}

void Win()
{
	int sleepTime = 1500;


	switch (level) {
	case 1:

		break;
	case 2:
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕1.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕2.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕3.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕4.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕5.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕6.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕7.jpg"));
		Sleep(sleepTime);
		loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\小蛇的归途_落幕8.jpg"));
		Sleep(sleepTime);
		break;
	}
}

void NextChapter()
{
	totalScore += score;
	if (level == 1) { 
		Win();return;
	}
	while(_kbhit())
		_getch();
	chapter++;
	if (chapter == 4) {
		Win();
		init();
	}
	else {
		cleanScreen();
		endFlag = false;
		score = 0;
		memset(map, 0, sizeof(map));
		loadMapImage();
		CreatMap();
		InitSnake();
		InitFood();
		_getch();
		control();
	}
}


void BrickRolling(void)
{
	int i, new_x[5], new_y[5];
	for (i = 2;i <= 4;i++) {
		new_x[i] = brick[1].x + (brick[i].y - brick[1].y) / 2;
		new_y[i] = brick[1].y - (brick[i].x - brick[1].x) * 2;
	}
	for (i = 2;i <= 4;i++) {
		map[brick[i].x][brick[i].y] = BLANK;
		printElement(BLANK, brick[i].x, brick[i].y, 0);
	}
	for (i = 2;i <= 4;i++) {
		brick[i].x = new_x[i];
		brick[i].y = new_y[i];
		map[brick[i].x][brick[i].y] = BRICK;
		printElement(BRICK, brick[i].x, brick[i].y, WHITE);
	}
}
void BrickFalling(char dir)
{
	int i, StopFlag = 0;;
	for (i = 1;i <= 4;i++) {
		printElement(BLANK, brick[i].x, brick[i].y, 0);
		map[brick[i].x][brick[i].y] = BLANK;
	}
	for (i = 1;i <= 4;i++)
		switch (dir) {
		case 'D':brick[i].x++;break;
		case 'L':brick[i].y -= 2;break;
		case 'R':brick[i].y += 2;break;
		}
	for (i = 1;i <= 4;i++) {
		if (map[brick[i].x][brick[i].y] == SNAKE) end();
		if (map[brick[i].x][brick[i].y] == FOOD) { InitFood();score -= 10; }
		if (brick[i].x == lines[level] - 2 || map[brick[i].x + 1][brick[i].y] == STOPPED_BRICK)  StopFlag = 1;
	}
	for (i = 1;i <= 4;i++) {
		map[brick[i].x][brick[i].y] = BRICK;
		printElement(BRICK, brick[i].x, brick[i].y,WHITE);
	}
	if (StopFlag) {
		for (i = 1;i <= 4;i++)  map[brick[i].x][brick[i].y] = STOPPED_BRICK;
		CheckBrick();
		InitBrick();
	}
}
void move(void)
{
	//map数组 0为无 1为有边界 2为有食物 3为炸弹 4为毒草 5为1号蛇身  6为砖头  7为停着的砖头
	int x, y;
	struct Snake *new_last, *current;

	stepcount++;
	if (level == 2) {
		if (stepcount % 100 == 0 ) InitGrass();
		if (chapter == 2 && stepcount % 30 == 0) {
			InitFood();
		}
	}


	memset(update, 0, sizeof(update));//1为从有到无，2为从无到有 
	current = head1->next;
	new_last = head1;
	if(current != NULL)
		while (current->next != NULL) {
			new_last = current;
			current = current->next;
		}
	switch (direction) {
	case 'U': {x = head1->x - 1;y = head1->y;}break;
	case 'D': {x = head1->x + 1;y = head1->y;}break;
	case 'L': {x = head1->x;y = head1->y - 2;}break;
	case 'R': {x = head1->x;y = head1->y + 2;}break;
	}
	if (map[x][y] == BORDER || map[x][y] == SNAKE || map[x][y] == BRICK)
		if (level != 4) {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\撞墙.wav"), NULL, SND_FILENAME | SND_ASYNC);
			end();
			if(level == 2||level == 3) rank();
			endFlag = true;
			return;
		}
		else {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\撞墙.wav"), NULL, SND_FILENAME | SND_ASYNC);
			EndPro(2);
			endFlag = true;
			return;
		}
		if (map[x][y] == FOOD) {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\吃食物.wav"), NULL, SND_FILENAME | SND_ASYNC);
			printscore(score, score + 10);
			if (level != 4) SnakeColor = NewColor;
			score += 10;
			length++;
			InitFood();
			update[x][y] = 2;
			map[x][y] = BLANK;
			struct Snake *newone;
			newone = (struct Snake*)malloc(sizeof(struct Snake));
			newone->x = x;
			newone->y = y;
			newone->next = head1;
			head1->last = newone;
			head1 = newone;
		}
		if (map[x][y] == BLANK) {
			update[x][y] = 2;
			map[x][y] = SNAKE;
			new_last->next = NULL;
			update[current->x][current->y] = 1;
			map[current->x][current->y] = BLANK;
			current->x = x;
			current->y = y;
			current->next = head1;
			head1 = current;
		}
		if (map[x][y] == POISON) {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\鬼.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (length <= 2) {
				end();
				if (level == 2 || level == 3) rank();
				endFlag = true;
				return;
			}
			length--;
			printscore(score, score - 5);
			score -= 5;
			new_last->next = NULL;
			update[current->x][current->y] = 1;
			map[current->x][current->y] = BLANK;
			free(current);
			map[x][y] = BLANK;
		}
		if (map[x][y] == WISDOM) {
			wisdomFlag = true;
			AiControl();
			wisdomFlag = false;
		}
		if (map[x][y] == BOMB) {
			PlaySound(_T("C:\\Users\\abc\\Desktop\\贪吃蛇\\声效\\吃炸弹.wav"), NULL, SND_FILENAME | SND_ASYNC);
			int temp = score;
			score -= length / 2 * 10;
			printscore(temp, score);
			if (length <= 2) {
				end();
				if (level == 2 || level == 3) rank();
				endFlag = true;
				return;
			}
			else {
				Snake *del, *temp;
				del = head1;
				length = (length % 2 == 0) ? (length / 2) : (length / 2 + 1);

				for (int i = 1; i < length; i++)
					del = del->next;
				temp = del;
				del = del->next;
				temp->next = NULL;
				while (del != NULL) {
					temp = del;
					update[del->x][del->y] = 1;
					del = del->next;
					free(temp);
				}
				map[x][y] = BLANK;
			}
		}
		print();
}
void move2(void)
{
	int x, y;
	struct Snake *new_last, *current;

	memset(update, 0, sizeof(update));//1为从有到无，2为从无到有 
	current = (head2->next);
	new_last = head2;
	while (current->next != NULL) {
		new_last = current;
		current = current->next;
	}
	switch (direction2) {
	case 'U': {x = head2->x - 1;y = head2->y;}break;
	case 'D': {x = head2->x + 1;y = head2->y;}break;
	case 'L': {x = head2->x;y = head2->y - 2;}break;
	case 'R': {x = head2->x;y = head2->y + 2;}break;
	}
	if (map[x][y] == BORDER || map[x][y] == SNAKE) EndPro(1);
	if (map[x][y] == FOOD) {
	
		update[x][y] = 4;
		map[x][y] = SNAKE;
		struct Snake *newone;
		newone = (struct Snake*)malloc(sizeof(struct Snake));
		newone->x = x;
		newone->y = y;
		newone->next = head2;
		head2 = newone;
		InitFood();
	}
	if (map[x][y] == BLANK) {
		update[x][y] = 4;
		map[x][y] = SNAKE;
		new_last->next = NULL;
		update[current->x][current->y] = 3;
		map[current->x][current->y] = BLANK;
		current->x = x;
		current->y = y;
		current->next = head2;
		head2 = current;
	}
	print();
}
void print(void)
{
	int i, j;
	if (level == 4) {
		if (stepcount == endcount) {
			int length1 = 0, length2 = 0;
			struct Snake *current;
			current = head1;
			while (current != NULL) {
				length1++;
				current = current->next;
			}
			current = head2;
			while (current != NULL) {
				length2++;
				current = current->next;
			}
			if (length1>length2) EndPro(1);
			if (length1<length2) EndPro(2);
			if (length1 == length2) EndPro(0);
		}
	}
	if (GlassFlag) {
		int mcolor;
		if (stepcount % 2 == 0) mcolor = CYAN;
		else mcolor = LIGHTGREEN;
		for (i = 1;i <= lines[level] - 2;i++)
			for (j = 2;j <= col[level] - 4;j += 2) {
				if (map[i][j] == POISON) {
					printElement(POISON, i, j, mcolor);
				}
				else if (map[i][j] == WISDOM) {
					printElement(WISDOM, i, j, color[stepcount%(color[0]-1)+1]);
				}
			}
	}
	for (i = 1;i <= lines[level] - 2;i++)
		for (j = 2;j <= col[level] - 4;j += 2)
			if (update[i][j] == 2) {
				printElement(SNAKE, i, j, SnakeColor);
				update[i][j] = 0;
			}
			else if (update[i][j] == 1) {
				printElement(BLANK, i, j, 0);
				update[i][j] = 0;
			}
	if (level == 4) {
		for (i = 1;i <= lines[level] - 2;i++)
			for (j = 2;j <= col[level] - 4;j += 2)
				if (update[i][j] == 4) {
					printElement(SNAKE, i, j, RED);
				}
				else if (update[i][j] == 3) {
					printElement(BLANK, i, j, 0);
				}
	}
}

void LoadGame()
{
	FILE *fin;

	fopen_s(&fin, "进度.txt", "r");
	fscanf_s(fin, "%d %d %d %c %d %d %d", &chapter, &score, &totalScore, &direction,&food.x,&food.y,&length);
	for (int i = 0; i < 40; i++)
		for (int j = 0; j < 80; j++)
			fscanf_s(fin, "%d ", &map[i][j]);
	Snake * current;
	head1 = (Snake*)malloc(sizeof(Snake));
	int x, y;
	fscanf_s(fin, "%d %d", &x, &y);
	head1->x = x;head1->y = y;
	current = head1;
	fscanf_s(fin, "%d %d", &x, &y);
	while(x != -1){
		current->next = (Snake*)malloc(sizeof(Snake));
		current = current->next;
		current->x = x;
		current->y = y;
		current -> next= NULL;
		fscanf_s(fin, "%d %d", &x, &y);
	}
	fclose(fin);

	level = 2;
	loadMapImage();
	for (int i = 0; i < 40; i++)
		for (int j = 0; j < 80; j += 2) {
			int color = WHITE;
			switch (map[i][j]) {
			case BORDER:
					color = WHITE;
					break;
				case SNAKE: 
					color = BLUE;
					break;
				case BOMB :
					color = RED;
					break;
				case FOOD :
					color = GREEN;
					break;
			}
			printElement(map[i][j], i, j, color);
		}
	printscore(0, score);
	while (_kbhit()) _getch();
	system("pause");
	control();
}

void SaveGame()
{
	FILE *fout;

	fopen_s(&fout, "进度.txt", "w");
	fprintf(fout, "%d %d %d %c %d %d %d\n", chapter, score, totalScore, direction,food.x,food.y,length);
	for (int i = 0; i < 40; i++)
		for (int j = 0; j < 80; j++)
			fprintf(fout, "%d ", map[i][j]);
	fprintf(fout, "\n");

	for (Snake *current = head1; current != NULL; current = current->next)
		fprintf(fout, "%d %d ", current->x, current->y);
	fprintf(fout, "-1 -1");
	fclose(fout);
}

int LoadOrNew()
{
	int current = 1;
	loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\新游戏.jpg"));
	while (1) {
		char ch = 0;
		if (_kbhit()) ch = _getch();
		if (ch == FIRSTKEY) ch = _getch();
		if (ch == UP && current>1) {
			current--;
			loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\新游戏.jpg"));
			Sleep(100);
		}
		if (ch == DOWN && current<2) {
			current++;
			loadimage(NULL, _T("C:\\Users\\abc\\Desktop\\贪吃蛇\\图片\\加载保存进度.jpg"));
			Sleep(100);
		}
		if (ch == ENTER) {
			break;
		}
	}
	return current;
}

