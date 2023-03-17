#pragma once
#include <ncurses.h>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

class Editor{
	// variables
	size_t x,y;
	char mode; // manipulate ncurses mode

	std::string filename, status, section;
	std::vector<std::string> lines;
	
	// functions
	void m_remove(int);
	std::string m_tabs(std::string&);
	void m_insert(std::string, int);
	void m_append(std::string&);

public:
	Editor(const std::string&);
	~Editor();
	void run();

protected:
	void update();
	void statusline();
	void input(int);
	void print();

	void up();
	void left();
	void right();
	void down();

	void open();
	void save();

};


inline void Editor::m_remove(int number) {lines.erase(lines.begin() + number);}

