#include<iostream>
#include<vector>
#include<raylib.h>

enum DIR {
	UP, DOWN, LEFT, RIGHT
};


DIR reverse(DIR &dir) {
	switch (dir)
	{
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	default:
		break;
	}
}


int width = 1000;
int height = 1000;

const int snakeSize = 10;


typedef struct snakeLimit {
	int top;
	int bottom;
	int left;
	int right;
} snakeLimit;
typedef struct SnakePoint {
	Vector2 pos;
	DIR dir;
} SnakePoint;

snakeLimit sl = snakeLimit{
	0,
	800,
	0,
	800
};
static void drawSnake(std::vector<SnakePoint> snakePoints) {
	int l = snakePoints.size();
	for (int i = 0; i < l - 1; i++) {
		int boxWidth = snakePoints[i + 1].pos.x - snakePoints[i].pos.x;
		if (boxWidth == 0) {
			boxWidth = snakeSize;
		}
		int boxHeight = snakePoints[i + 1].pos.y - snakePoints[i].pos.y;
		if (boxHeight == 0) {
			boxHeight = snakeSize;
		}
		DrawRectangleV(snakePoints[i].pos, Vector2{ (float)boxWidth,(float)boxHeight }, RED);
	}
}


template <class T>
std::vector<T> insertAt(T value, size_t index) {

}


void moveSnake(std::vector<SnakePoint>& snakePoints, DIR dir) {
	SnakePoint &head = snakePoints[0];
	if (reverse(head.dir) != dir) {
		head.dir == dir;
		switch (dir)
		{
		case UP:
				head.pos.y -= snakeSize;
				if (head.dir != dir) {

				}
			break;
		case DOWN:
			head.pos.y += snakeSize;
			break;
		case LEFT:
			head.pos.x -= snakeSize;
			break;
		case RIGHT:
			head.pos.x += snakeSize;
			break;
		default:
			break;
		}
	}
	
}

int main() {
	InitWindow(800, 800, "Mdhe Snake");
	std::vector<SnakePoint> snakePoints = { SnakePoint{
		Vector2{400,400},
		UP
		},
		SnakePoint{
	Vector2{400,700},
	UP}
	};
	SetTargetFPS(60);
	DIR snake_dir = UP;
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GREEN);
		drawSnake(snakePoints);
		EndDrawing();

	}
}