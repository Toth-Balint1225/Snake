#include "GameEngine.h"

class Game : public GameEngine {
public:
	bool onCreate() {
	}

	bool onUpdate() {
	}

	bool onDestroy() {
	}
private:
	bool onDraw(const Cairo::RefPtr<Cairo::Context>& cr) {
	}
};

int main() {
	Game game;
	game.constructWindow(100,100,10,"Snake");
	game.start(30);
	return 0;
}
