#include <cstdlib>
#include <algorithm>
#include <list>
#include <string>
#include "GameEngine.h"

#define HW_RIGHT 40
#define HW_UP 25
#define HW_LEFT 38
#define HW_DOWN 39

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
	float deltaTime;
	bool gameOver;
	Pos apple;
	bool appleOnScreen;
public:
	Game(int _width, int _height, int _projector, const std::string& _title): gg::GameEngine(_width,_height,_projector,_title) {
		gg::GamingWindow::gamingWindow.signal_key_press_event().connect(sigc::mem_fun(*this,&Game::onKeyPress),false);	
	}

	virtual ~Game() {
	}
private:
	void turnSnake(Direction direction) {
		switch (direction) {
		case RIGHT:
			snake.push_front({snake.front().x+1,snake.front().y});
			break;
		case UP:
			snake.push_front({snake.front().x,snake.front().y-1});
			break;
		case LEFT:
			snake.push_front({snake.front().x-1,snake.front().y});
			break;
		case DOWN:
			snake.push_front({snake.front().x,snake.front().y+1});
			break;
		}
	}
	bool generateApple() {
		bool appleGenerated = false;
		while (!appleGenerated) {
			int x = rand()%width;
			int y = rand()%height;
			if (std::find_if(snake.begin(),snake.end(),[x,y](const Pos& p) {
				return p.x != x || p.y != y;
			}) != snake.end()) {
				apple = {x,y};
				appleGenerated = true;
				appleOnScreen = true;
			}
		}
		return true;
	}
public:
	virtual bool onCreate() override {
		srand(time(NULL));
		snake = {{5,1},{4,1},{3,1},{2,1},{1,1}};
		snakeDirection = RIGHT;
		snakeVelocity = 5.f;
		deltaTime = 0.f;
		gameOver = false;
		appleOnScreen = false;
		return true;
	}

	virtual bool onUpdate(float elapsed) override {
		deltaTime += (elapsed*snakeVelocity);
		if (deltaTime >= 1.f) {
			turnSnake(snakeDirection);
			//wall collision detection
			if (snake.front().x < 0 || snake.front().x >= width || snake.front().y < 0 || snake.front().y >= height)
				gameOver = true;

			// collision in itself detection
			for (auto it = snake.begin();it!=snake.end();++it) {
				if (it != snake.begin() && it->x == snake.front().x && it->y == snake.front().y) {
					gameOver = true;
					break;
				}
			}
			deltaTime = 0.f;
			snake.pop_back();
		}
		if (!appleOnScreen) {
			generateApple();
		}
		if (gameOver) {
			gg::GamingWindow::gamingWindow.close();
		} 
		return true;
	}

	virtual bool onDestroy() override {
		return true;
	}
private:
	virtual bool onDraw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		fillRect(cr,0,0,width*pixelSize,height*pixelSize,0.f,0.f,0.f);
		for (auto it : snake) {
			fillRect(cr,(it.x)*pixelSize,(it.y)*pixelSize,pixelSize,pixelSize,0.f,1.f,0.f);
		}
		fillRect(cr,apple.x*pixelSize,apple.y*pixelSize,pixelSize,pixelSize,1.f,0.f,0.f);
		grid(cr);
		return true;
	}
	bool onKeyPress(GdkEventKey* event) {
		if (event->hardware_keycode == HW_RIGHT && snakeDirection != LEFT) {
			snakeDirection = RIGHT;
			onUpdate(1.f);
		}
		if (event->hardware_keycode == HW_UP && snakeDirection != DOWN) {
			snakeDirection = UP;
			onUpdate(1.f);
		}
		if (event->hardware_keycode == HW_LEFT && snakeDirection != RIGHT) {
			snakeDirection = LEFT;
			onUpdate(1.f);
		}
		if (event->hardware_keycode == HW_DOWN && snakeDirection != UP) {
			snakeDirection = DOWN;
			onUpdate(1.f);
		}
		return false;	
	}
	void fillRect(const Cairo::RefPtr<Cairo::Context>& cr,float x1,float y1,float w,float h,float r,float g,float b) {
		cr->save();
		cr->set_source_rgb(r,g,b);
		cr->rectangle(x1,y1,w,h);
		cr->fill_preserve();
		cr->stroke();
		cr->restore();
	}
	void grid(const Cairo::RefPtr<Cairo::Context>& cr) {
		cr->save();
		cr->set_source_rgb(0.f,0.f,0.f);
		for (int i=0;i<=width;i++) {
			cr->move_to(i*pixelSize,0);
			cr->line_to(i*pixelSize,height*pixelSize);
		}
		for (int i=0;i<=height;i++) {
			cr->move_to(0,i*pixelSize);
			cr->line_to(width*pixelSize,i*pixelSize);
		}

		cr->stroke();
		cr->restore();
	}
};

int main() {
	Game game(30,20,10,"Snake");
	game.start(30);
	return 0;
}
