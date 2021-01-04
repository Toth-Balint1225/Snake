#include <list>
#include <string>
#include "GameEngine.h"

class Game : public gg::GameEngine {
private:
	struct Pos {
		int x,y;
	};
	
	std::list<Pos> snake;
public:
	Game(int _width, int _height, int _projector, const std::string& _title): gg::GameEngine(_width,_height,_projector,_title) {
	}

	virtual ~Game() {
	}
public:
	virtual bool onCreate() override {
		return true;
	}

	virtual bool onUpdate() override {
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
	Game game(10,10,10,"Snake");
	game.start(30);
	return 0;
}
