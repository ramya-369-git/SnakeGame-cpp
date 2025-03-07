#include<iostream>
#include<raylib.h>
#include<deque>
#include <raymath.h>
using namespace std;

Color  blue = { 90, 180, 204, 255 };
Color darkblue = { 0, 51, 102, 255 };
int cellsize = 30;
int cellcount = 25;
double LastUpdateTime = 0;
int offset = 75;

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - LastUpdateTime >= interval) {
		LastUpdateTime = currentTime;
		return true;
	}
	return false;
}
bool ElementInDeque(Vector2 element, deque<Vector2> dq)
{
	for (int i = 0; i < dq.size(); i++)
	{
		if (Vector2Equals(dq[i], element))
			return true;
	}
	return false;
}
class Food
{
public:
	Vector2 position;
	Food(deque<Vector2> dq) {
		position = GenerateRandomPos(dq);
	}
	void Draw()
	{
		DrawRectangle(offset + position.x * cellsize, offset + position.y * cellsize, cellsize, cellsize, darkblue);
	}
	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellcount - 1);
		float y = GetRandomValue(0, cellcount - 1);
		return Vector2{ x,y };
	}
	Vector2 GenerateRandomPos(deque<Vector2> dq)
	{
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, dq))
		{
			position = GenerateRandomCell();
		}
		return position;
	}
};
class Snake
{
public:
	deque<Vector2> snakebody = { Vector2{6,9},Vector2{5,9},Vector2{4,9} };
	Vector2 direction = { 1,0 };
	bool addSegment = false;
	void Draw()
	{
		for (int i = 0; i < snakebody.size(); i++)
		{
			float x = snakebody[i].x;
			float y = snakebody[i].y;
			Rectangle rec = { offset + x * cellsize,offset + y * cellsize,cellsize,cellsize };
			DrawRectangleRounded(rec, 0.75f, 8, darkblue);
		}
	}
	void Update()
	{
		snakebody.push_front(Vector2Add(snakebody[0], direction));
		if (addSegment == true)
			addSegment = false;
		else
			snakebody.pop_back();
	}
	void Reset()
	{
		snakebody = { Vector2{6,9},Vector2{5,9},Vector2{4,9} };
		Vector2 direction = { 1,0 };
	}
};
class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.snakebody);
	bool Running = true;
	int score = 0;
	void Draw()
	{
		food.Draw();
		snake.Draw();
	}
	void Update()
	{
		if (Running)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}
	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.snakebody[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.snakebody);
			snake.addSegment = true;
			score++;
		}
	}
	void CheckCollisionWithEdges()
	{
		if (snake.snakebody[0].x == cellcount || snake.snakebody[0].x == -1)
		{

			GameOver();
		}
		if (snake.snakebody[0].y == cellcount || snake.snakebody[0].y == -1)
		{
			GameOver();
		}
	}
	void CheckCollisionWithTail()
	{
		deque<Vector2> headlessbody = snake.snakebody;
		headlessbody.pop_front();
		if (ElementInDeque(snake.snakebody[0], headlessbody))
			GameOver();
	}
	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.snakebody);
		Running = false;
		score = 0;
	}
};
int main()
{
	InitWindow(2 * offset + cellsize * cellcount, 2 * offset + cellsize * cellcount, "Snake Game...!");
	SetTargetFPS(60);
	Game game;
	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		if (eventTriggered(0.2))
			game.Update();
		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
			game.snake.direction = { 0,-1 };
			game.Running = true;
		}
		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
			game.snake.direction = { 0,1 };
			game.Running = true;
		}
		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
			game.snake.direction = { -1,0 };
			game.Running = true;
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
			game.snake.direction = { 1,0 };
			game.Running = true;
		}
		ClearBackground(blue);
		Rectangle rec = { (float)offset ,(float)offset ,(float)cellsize * cellcount,(float)cellsize * cellcount };
		DrawRectangleLinesEx(rec, 5, darkblue);
		DrawText("Snake Game...!", offset, offset - 40, 40, darkblue);
		DrawText(TextFormat("Score :%i", game.score), offset, cellsize * cellcount + offset, 40, darkblue);
		game.Draw();
		EndDrawing();

	}
	CloseWindow();
}