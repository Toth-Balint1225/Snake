#include <iostream>
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
	float deltaTime;
public:
	Game(int _width, int _height, int _projector, const std::string& _title): gg::GameEngine(_width,_height,_projector,_title) {
		gg::GamingWindow::gamingWindow.signal_key_press_event().connect(sigc::mem_fun(*this,&Game::onKeyPress),false);	
	}

	virtual ~Game() {
	}
public:
	virtual bool onCreate() override {
		snake = {{1,18}};
		snakeDirection = RIGHT;
		snakeVelocity = 1.5f;
		deltaTime = 0.f;
		return true;
	}

	virtual bool onUpdate(float elapsed) override {
		deltaTime += (elapsed*snakeVelocity);
		if (deltaTime >= 1.f) {
			switch (snakeDirection) {
			case RIGHT:
				snake.front().x += 1;
				break;
			case UP:
				snake.front().y -= 1;
				break;
			case LEFT:
				snake.front().x -= 1;
				break;
			case DOWN:
				snake.front().y += 1;
				break;
			}
			deltaTime = 0.f;
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
		grid(cr);
		return true;
	}
	bool onKeyPress(GdkEventKey* event) {
		std::cout << event->hardware_keycode << std::endl;
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
