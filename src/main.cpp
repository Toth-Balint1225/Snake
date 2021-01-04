#include <list>
#include <string>
#include "GameEngine.h"

class Game : public gg::GameEngine {
private:
	struct Pos {
		int x,y;
	};
	enum Direction {
		RIGHT,UP,LEFT,DOWN
	};
	
	std::list<Pos> snake;
	float snakeVelocity;
	Direction snakeDirection;
public:
	Game(int _width, int _height, int _projector, const std::string& _title): gg::GameEngine(_width,_height,_projector,_title) {
	}

	virtual ~Game() {
	}
public:
	virtual bool onCreate() override {
		snake = {{width/2,height/2}};
		snakeDirection = RIGHT;
		snakeVelocity = 1.f;
		deltaTime = 0.f;
		return true;
	}

	virtual bool onUpdate(float elapsed) override {
		return true;
	}

	virtual bool onDestroy() override {
		return true;
	}
private:
	virtual bool onDraw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		return true;
	}
};

int main() {
	Game game(30,20,10,"Snake");
	game.start(30);
	return 0;
}
