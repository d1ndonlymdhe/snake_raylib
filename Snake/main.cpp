#include<iostream>
#include<vector>
#include<raylib.h>
#include<functional>
#include<string>
enum DIR {
	UP, DOWN, LEFT, RIGHT
};

template<class T>
class Node {
public:
	T value;
	Node<T>* next = NULL;
	Node<T>* prev = NULL;
	Node(T v, Node<T>* p, Node<T>* n) {
		value = v;
		prev = p;
		next = n;
	}
	Node(T v, Node<T>* p) {
		value = v;
		prev = p;
	}
	Node(T v) {
		value = v;
	}
	Node<T>* getLast() {
		Node<T>* last = this;
		while (last->next != NULL) {
			last = last->next;
		}
		return last;
	}
	Node<T>* addNodeHere(Node<T>* n) {
		if (this->next == NULL) {
			this->next = n;
		}
		else {
			Node<T>* temp = this->next;
			n->getLast()->next = temp;
			temp->prev = n;
			this->next = n;
		}
		n->prev = this;
		return n;
	}
	Node<T>* addNodeHere(T n) {
		Node<T>* newNode = new Node(n);
		if (this->next == NULL) {
			this->next = newNode;
		}
		else {
			Node<T>* currentNextNode = this->next;
			newNode->getLast()->next = currentNextNode;
			currentNextNode->prev = newNode;
			this->next = newNode;
		}
		newNode->prev = this;
		return newNode;
	}
	template<class R>
	R run(std::function<R(Node<T>*)> func) {
		return func(this);
	};
	void run(std::function<void(Node<T>*)> func) {
		func(this);
	}
	void forEach(std::function<void(Node<T>*)> func) {
		func(this);
		Node<T>* next = this->next;
		while (next != NULL) {
			func(next);
			next = next->next;
		}
	}
	Node<T>* find(std::function<bool(Node<T>*)> func) {
		bool res = func(this);
		if (res) {
			return this;
		}
		Node<T>* next = this->next;
		while (next != NULL) {
			bool res = func(next);
			if (res) {
				return next;
			}
			next = next->next;
		}
		return NULL;
	}
	void remove() {
		this->prev->next = this->next;
		this->next = this->prev;
		delete this;
	};
};
int getTextCenterX(const char* str, int fontSize, int width, int offset = 0) {
	int x = MeasureText(str, fontSize);
	return (width / 2) - (x / 2) + offset;
}
int getTextCenterY(const char* str, int fontSize, int height, int offset = 0) {
	std::string temp = str;
	return (height / 2) - ((fontSize * temp.length()) / 2) + offset;
}
DIR reverse(DIR& dir) {
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
	}
}
bool operator==(Vector2 A, Vector2 B) {
	if (A.x == B.x && A.y == B.y) {
		return true;
	}
	return false;
}
int width = 800;
int height = 800;
const int snakeSize = 10;
typedef struct snakeLimit {
	int top;
	int bottom;
	int left;
	int right;
} snakeLimit;
class SnakePoint {
public:
	Vector2 pos;
	DIR dir;
	SnakePoint clone() {
		return SnakePoint{
			pos,
			dir
		};
	}
};
typedef Node<SnakePoint> SnakeNode;
SnakeNode CloneSnake(SnakeNode snakePoint) {
	SnakePoint point = snakePoint.value.clone();
	SnakeNode newSnake(point);
	SnakeNode* newSnakeLast = &newSnake;
	SnakeNode* next = (snakePoint.next);
	while (next != NULL) {
		newSnakeLast->addNodeHere(new SnakeNode(next->value.clone()));
		newSnakeLast = newSnakeLast->next;
		next = next->next;
	}
	return newSnake;
}
static bool isPointInsideSnake(Vector2 point, SnakeNode Head) {
	auto collisionDetector = [point](SnakeNode* node)->bool {
		SnakeNode* next = node->next;
		if (next != NULL) {
			Rectangle collisionRect = {
				0,0,0,0 };
			Vector2 currentPos = node->value.pos;
			Vector2 nextPos = next->value.pos;
			float boxWidth = nextPos.x - currentPos.x;
			float boxHeight = nextPos.y - currentPos.y;
			if (boxHeight < 0.0f || boxWidth < 0.0f) {

				collisionRect = {
					nextPos.x,
					nextPos.y,
					abs(boxWidth),
					abs(boxHeight)
				};
			}
			else {
				collisionRect = {
					currentPos.x,
					currentPos.y,
					abs(boxWidth),
					abs(boxHeight)
				};
			}
			return CheckCollisionPointRec(point, collisionRect);
		}
		return false;
		};
	if (Head.find(collisionDetector) == NULL) {
		return false;
	}
	return true;
}
void drawSnake(Node<SnakePoint> Head) {
	Node<SnakePoint>& currentNode = Head;
	while (currentNode.next != NULL) {
		Vector2 currentPos = currentNode.value.pos;
		Vector2 nextPos = currentNode.next->value.pos;
		float boxWidth = nextPos.x - currentPos.x;
		if (boxWidth == 0.0f)
		{
			boxWidth = snakeSize;
		}
		float boxHeight = nextPos.y - currentPos.y;
		if (boxHeight == 0.0f) {
			boxHeight = snakeSize;
		}
		currentNode = *currentNode.next;
		if (boxHeight < 0.0f || boxWidth < 0.0f) {
			float wSupplement = boxWidth < 0.0f ? snakeSize : 0.0f;
			float hSupplement = boxHeight < 0.0f ? snakeSize : 0.0f;
			float wSupplement2 = (boxWidth >= snakeSize && boxHeight == snakeSize) ? snakeSize : 0.0f;
			DrawRectangleV(nextPos, Vector2{ abs(boxWidth) + wSupplement,abs(boxHeight) + hSupplement }, RED);
		}
		else if (boxHeight == 10.0f && boxWidth == 10.0f) {
			DrawRectangleV(currentPos, Vector2{ abs(boxWidth),abs(boxHeight) }, RED);
		}
		else {
			float wSupplement = (boxWidth >= snakeSize && boxHeight == snakeSize) ? snakeSize : 0.0f;
			//DrawRectangleV(currentPos, Vector2{ abs(boxWidth) + wSupplement,abs(boxHeight) }, RED);
			DrawRectangleV(currentPos, Vector2{ abs(boxWidth),abs(boxHeight) }, RED);
		}

	}
}
void moveSnake(SnakeNode& Head, DIR dir) {
	SnakePoint& head = Head.value;
	SnakeNode& Tail = *(Head.getLast());
	SnakePoint& tail = Tail.value;

	SnakePoint headClone = head.clone();
	bool dirChanged = false;
	if (reverse(head.dir) != dir) {
		if (head.dir != dir) {
			head.dir = dir;
			dirChanged = true;
		}
		switch (dir)
		{
		case UP:
			head.pos.y -= snakeSize;
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
	if (dirChanged) {
		headClone.dir = dir;
		Head.addNodeHere(new SnakeNode(headClone));
	}
	SnakeNode* BeforeTail = Tail.prev;
	SnakePoint beforeTail = BeforeTail->value;
	if (reverse(head.dir) != dir) {
		if (head.dir != dir) {
			dirChanged = true;
		}
		switch (tail.dir)
		{
		case UP:
			tail.pos.y -= snakeSize;
			break;
		case DOWN:
			tail.pos.y += snakeSize;
			break;
		case LEFT:
			tail.pos.x -= snakeSize;
			break;
		case RIGHT:
			tail.pos.x += snakeSize;
			break;
		default:
			break;
		}
		if ((tail.dir == UP || tail.dir == DOWN) && tail.pos.y == beforeTail.pos.y) {
			Tail.remove();
		}
		if ((tail.dir == LEFT || tail.dir == RIGHT) && tail.pos.x == beforeTail.pos.x) {
			Tail.remove();
		}
	}
}
Vector2 generateApple(SnakeNode& Head) {
	SnakeNode& Tail = *(Head.getLast());
	SnakePoint tail = Tail.value.clone();
	Vector2 genPoint;
	bool notSafe = true;
	do
	{
		int randX = ((int)(rand() % 801 + 1) / 10) * 10;
		int randY = ((int)(rand() % 801 + 1) / 10) * 10;
		genPoint = Vector2{
			(float)randX,(float)randY
		};
		notSafe = isPointInsideSnake(genPoint, Head);
	} while (notSafe);
	return genPoint;
}
void eatApple(SnakeNode& Head, Vector2 ApplePos) {
	Head.value.pos = ApplePos;
	SnakeNode* Tail = Head.getLast();
	SnakePoint& tail = Tail->value;
	if (tail.dir == UP || tail.dir == DOWN) {
		tail.pos.y += (tail.dir == UP) ? +snakeSize : -snakeSize;
	}
	else {
		tail.pos.x += (tail.dir == LEFT) ? +snakeSize : -snakeSize;
	}
}
void drawGrid() {
	for (auto i = 0; i < width; i += snakeSize) {
		if (i % 100 == 0) {
			DrawLine(i, 0, i, height, YELLOW);
		}
		else {
			DrawLine(i, 0, i, height, ColorAlpha(YELLOW, 0.7));

		}
	}
	for (auto i = 0; i < height; i += snakeSize) {
		if (i % 100 == 0) {
			DrawLine(0, i, width, i, YELLOW);
		}
		else {
			DrawLine(0, i, width, i, ColorAlpha(YELLOW, 0.7));

		}
	}
}
int main() {
	srand(time(0));
	InitWindow(800, 800, "Mdhe Snake");
	SnakePoint Head = SnakePoint{
		Vector2{
			200,200
		},
		UP
	};
	SnakePoint Tail = SnakePoint{
		Vector2{
			200,400
		},
		UP
	};
	Node<SnakePoint> snakePoints(Head);
	snakePoints.addNodeHere(Tail);
	SetTargetFPS(30);
	bool move = true;
	bool gameOver = false;
	bool grid = false;
	int points = 0;
	Vector2 apple = generateApple(snakePoints);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GREEN);
		drawSnake(snakePoints);
		SnakeNode tempSnake = CloneSnake(snakePoints);
		SnakePoint headClone = tempSnake.value.clone();
		SnakePoint& headRef = tempSnake.value;
		if (grid) {
			drawGrid();
		}
		if (IsKeyPressed(KEY_D)) {
			grid = !grid;
		}
		if (headRef.dir == UP || headRef.dir == DOWN) {
			headRef.pos.y += (headRef.dir == UP) ? snakeSize : -snakeSize;
		}
		else {
			headRef.pos.x += (headRef.dir == LEFT) ? snakeSize : -snakeSize;
		};

		if (isPointInsideSnake(headClone.pos, tempSnake)) {
			gameOver = true;
		}
		if (!gameOver) {
			if (IsKeyPressed(KEY_G)) {
				apple = generateApple(snakePoints);
			}
			if (IsKeyPressed(KEY_P)) {
				move = !move;
				std::function<void(SnakeNode*)> print = [](SnakeNode* node)->void {
					std::cout << node->value.pos.x << " " << node->value.pos.y << std::endl;
					};
				snakePoints.forEach(print);
			}if (IsKeyPressed(KEY_E)) {
				eatApple(snakePoints, Vector2{ 200,100 });
			}
			if (IsKeyPressed(KEY_UP)) {
				moveSnake(snakePoints, UP);
			}
			else if (IsKeyPressed(KEY_LEFT)) {
				moveSnake(snakePoints, LEFT);
			}
			else if (IsKeyPressed(KEY_DOWN)) {
				moveSnake(snakePoints, DOWN);
			}
			else if (IsKeyPressed(KEY_RIGHT)) {
				moveSnake(snakePoints, RIGHT);
			}
			else {
				if (move) {
					moveSnake(snakePoints, snakePoints.value.dir);
				}
			}
			if (snakePoints.value.pos == apple) {
				eatApple(snakePoints, apple);
				apple = generateApple(snakePoints);
				points++;
			}
		}

		if (snakePoints.value.pos.x < 0 || snakePoints.value.pos.x > width || snakePoints.value.pos.y <0 || snakePoints.value.pos.y > height) {
			gameOver = true;
		}
		if (gameOver) {
			const char* text = "Game Over";
			int fontSize = 50;
			int x = getTextCenterX(text, fontSize, width, 0);
			DrawText(text, x, 50, fontSize, BLACK);
			const char* text2 = "Press enter to restart";
			x = getTextCenterX(text2, fontSize, width, 0);
			DrawText(text2, x, 100, fontSize, BLACK);
			if (IsKeyPressed(KEY_ENTER)) {
				snakePoints =  SnakeNode(Head);
				snakePoints.addNodeHere(Tail);
				apple = generateApple(snakePoints);
				points = 0;
				gameOver = false;
			}
		}
		{
			std::string pointsString = std::to_string(points);
			const char* text = pointsString.c_str();
			int x = getTextCenterX(text, 200, width, 0);
			int y = getTextCenterY(text, 200, height, 0);
			DrawText(text, x, y, 200, ColorAlpha(WHITE, 0.5));
		}
		DrawRectangleV(apple, Vector2{ 10,10 }, BLUE);
		EndDrawing();

	}
}