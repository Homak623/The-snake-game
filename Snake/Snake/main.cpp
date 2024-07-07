#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

using namespace std;

#define HEIGHT 30
#define WIDTH 80
#define ftail 219
#define fhead 79


enum Direction { dright, dleft, up, down };
enum TResult { Kill, Ok };

void MoveXY(int x, int y) {
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

struct TMap {
	char map1[HEIGHT][WIDTH + 1];
	void Show();
	void Clear();
};
struct apple {
	int Applessum;
	POINT apples[100];
};
class Snake {
	int x, y;
	TMap* mp;
	POINT tail[100];
	int len;
	Direction dir;
public:
	void AddToTail(int x, int y);
	void MoveTail();
	Snake(TMap* map);
	void Init(int _x, int _y, Direction _dir);
	TResult Direct(char w, char a, char s, char d, apple& app);
	void PutOnMap(apple& app);
	void CheckApples(apple& app);
	void Apples(apple& app);
	void DeleteApples(apple& app);
};

void Snake::MoveTail() {
	for (int i = len - 1; i >= 0; i--) {
		tail[i + 1] = tail[i];
	}
	tail[0].x = x;
	tail[0].y = y;
}

void Snake::AddToTail(int x, int y) {
	len++;
	tail[len - 1].x = x;
	tail[len - 1].y = y;
}

Snake::Snake(TMap* map) {
	mp = map;
	Init(0, 0, up);
}
void Snake::Init(int _x, int _y, Direction _dir) {
	x = _x;
	y = _y;
	dir = _dir;
}

void Snake::PutOnMap(apple& app) {
	mp->map1[x][y] = fhead;
	for (int i = 0; i < len; i++) {
		mp->map1[tail[i].x][tail[i].y] = ftail;
	}
	for (int i = 0; i < app.Applessum; i++) {
		mp->map1[app.apples[i].x][app.apples[i].y] = '*';
	}
}
void TMap::Show() {
	MoveXY(0, 0);
	map1[HEIGHT - 1][WIDTH - 1] = '\0';
	for (int i = 0; i < HEIGHT; i++) {
		if (i == HEIGHT - 1)
			break;
		cout << map1[i] << endl;
	}
}

void TMap::Clear() {
	for (int i = 0; i < WIDTH; i++) {
		map1[0][i] = ' ';
	}
	map1[0][WIDTH] = '\0';
	for (int i = 1; i < HEIGHT; i++) {
		strncpy(map1[i], map1[0], WIDTH + 1);
	}
}
int k = 0;

TResult Snake::Direct(char w, char a, char s, char d, apple& app) {
	MoveTail();
	POINT old;
	old.x = x;
	old.y = y;
	if (GetKeyState(w) < 0) {
		dir = up;
	}
	if (GetKeyState(a) < 0)
	{
		dir = dleft;
	}
	if (GetKeyState(s) < 0) {
		dir = down;
	}
	if (GetKeyState(d) < 0) {
		dir = dright;
	}
	if (dir == up)x--;
	if (dir == down)x++;
	if (dir == dright)y++;
	if (dir == dleft)y--;

	if (mp->map1[x][y] == '*') {
		AddToTail(old.x, old.y);
		DeleteApples(app);
		Apples(app);
	}
	for (int i = 0; i < len; i++) {
		if (x == tail[i].x && y == tail[i].y) {
			return Kill;
		}
	}

	if (x == HEIGHT || x == 0 || y == WIDTH || y == 0) {
		return Kill;
	}
	else
		return Ok;
}

void Snake::Apples(apple& app) {
	app.Applessum++;
	app.apples[app.Applessum - 1].x = rand() % 20 + 3;
	app.apples[app.Applessum - 1].y = rand() % 60 + 3;
	CheckApples(app);
	mp->map1[app.apples[app.Applessum - 1].x][app.apples[app.Applessum - 1].y] = '*';
}

void Snake::CheckApples(apple& app) {
	for (int i = 0; i < app.Applessum; i++) {
		if (i != app.Applessum - 1 && app.apples[app.Applessum - 1].x == app.apples[i].x && app.apples[app.Applessum - 1].y == app.apples[i].y) {
			app.apples[app.Applessum - 1].x = rand() % 20 + 3;
			app.apples[app.Applessum - 1].y = rand() % 60 + 3;
			i = 0;
		}
	}
}

void Snake::DeleteApples(apple& app) {
	int index;
	for (int i = 0; i < app.Applessum; i++) {
		if (app.apples[i].x == x && app.apples[i].y == y) {
			index = i;
			break;
		}
	}
	app.apples[index].x = app.apples[app.Applessum - 1].x;
	app.apples[index].y = app.apples[app.Applessum - 1].y;
	app.Applessum--;
}
int main() {
	TMap t;
	apple app;
	app.Applessum = 0;
	Snake Snake2(&t);
	Snake Snake(&t);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Apples(app);
	Snake.Init(2, 3, dright);
	Snake2.Init(2, 60, dleft);
	do {
		t.Clear();
		Snake.PutOnMap(app);
		Snake2.PutOnMap(app);
		if (Snake.Direct('W', 'A', 'S', 'D', app) == Kill)
			Snake.Init(2, 3, dright);
		if (Snake2.Direct(VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, app) == Kill)
			Snake2.Init(2, 60, dleft);
		t.Show();
		Sleep(80);
	} while ((GetKeyState(VK_ESCAPE) <= 0));
	return 0;
}

