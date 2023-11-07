#include<iostream>
#include<vector>
#include<raylib.h>

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
	void remove() {
		this->prev->next = this->next;
		this->next = this->prev;
		delete this;
	}
};

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

snakeLimit sl = snakeLimit{
	0,
	800,
	0,
	800
};
typedef Node<SnakePoint> SnakeNode;

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
			DrawRectangleV(nextPos, Vector2{ abs(boxWidth) + wSupplement,abs(boxHeight) }, RED);
		}
		else {
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
		if ((tail.dir == UP || tail.dir == DOWN) &&  tail.pos.y == beforeTail.pos.y) {
			Tail.remove();
			//BeforeTail->remove();
		}
		if ((tail.dir == LEFT || tail.dir == RIGHT) && tail.pos.x == beforeTail.pos.x) {
			//BeforeTail->remove();
			Tail.remove();
		}

	}




}

int main() {
	InitWindow(800, 800, "Mdhe Snake");
	SnakePoint Head = SnakePoint{
		Vector2{
			400,400
		},
		UP
	};
	SnakePoint Tail = SnakePoint{
		Vector2{
			400,700
		},UP
	};
	SnakePoint TailTail = SnakePoint{
		Vector2{
			200,700
		},
		RIGHT
	};
	SnakePoint TailTailTail = SnakePoint{
		Vector2{
			200,400
		},
		DOWN
	};
	Node<SnakePoint> snakePoints(Head);
	snakePoints.addNodeHere(new SnakeNode(Tail, &snakePoints));
	SetTargetFPS(30);
	DIR snake_dir = UP;
	bool x = (snakePoints.next->next == NULL);
	std::cout << x << std::endl;
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GREEN);
		drawSnake(snakePoints);
		moveSnake(snakePoints, LEFT);
		EndDrawing();

	}
}