#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>

#include <glibmm/main.h>

#include <cairomm/context.h>

// The Ghetto Game engine
namespace gg {

class GameEngine;
class GamingWindow {
public:
	static void show(GameEngine& game);
private:
	static Glib::RefPtr<Gtk::Application> gamingApp;
	static Gtk::Window gamingWindow;
};


class GameEngine : public Gtk::DrawingArea {
private:
	float refreshTimeout = 0.f;
protected:
	int width;
	int height;
	int pixelSize;
	std::string title;
public:
	friend class GamingWindow;
	GameEngine() {
	}
	GameEngine(int _width, int _height, int _pixelSize, const std::string& _title) {
		width = _width; 
		height = _height; 
		pixelSize = _pixelSize; 
		title = _title; 
	}
	virtual ~GameEngine() override {
	}
	void constructWindow(int _width, int _height, int _pixelSize, const std::string& _title) {
		width = _width; 
		height = _height; 
		pixelSize = _pixelSize; 
		title = _title; 
	}
public:
	void start(float timeout) {
		Glib::signal_timeout().connect(sigc::mem_fun(*this,&GameEngine::frame),timeout);
		refreshTimeout = timeout/1000;
		GamingWindow::show(*this);
	}
protected:
	bool refresh() {
		auto win = get_window();
		if (win) {
			Gdk::Rectangle screen(0,0,get_allocation().get_width(), get_allocation().get_height());
			win->invalidate_rect(screen, false);
		}
		return true;
	}
private:
	bool frame() {
		bool active = onUpdate(refreshTimeout);
		refresh();
		return active;
	}
private:
	virtual bool onDraw(const Cairo::RefPtr<Cairo::Context>& cr) = 0;
public:
	virtual bool onCreate() = 0;
	virtual bool onUpdate(float elapsed) = 0;
	virtual bool onDestroy() = 0;
protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		onDraw(cr);
		return true;
	}
};

void GamingWindow::show(GameEngine& game) {
	gamingWindow.set_title(game.title);
	gamingWindow.resize(game.pixelSize*game.width, game.pixelSize*game.height);
	if (!game.onCreate())
		return;
	gamingWindow.add(game);
	game.set_size_request(game.pixelSize*game.width, game.pixelSize*game.height);
	game.show();
	gamingApp->run(gamingWindow);
	game.onDestroy();
}
Glib::RefPtr<Gtk::Application> GamingWindow::gamingApp = Gtk::Application::create();
Gtk::Window GamingWindow::gamingWindow;

} // ns gg

#endif //GAMEENGINE_H
