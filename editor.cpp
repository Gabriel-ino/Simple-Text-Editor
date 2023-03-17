#include "editor.hpp"

Editor::Editor(const std::string& file){
	lines.push_back("");
	x = y = 0;
	mode = 'n';
	status = "NORMAL";
	section = "";
	if (file.empty()){
		filename = "Untitled";
	}else{
		filename = file;
	}

	open();

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, true);
	use_default_colors();

}

Editor::~Editor(){
	refresh();
	endwin();

}

void Editor::run(){
	//printw(&filename[0]);
	//getch();
	while (mode != 'q'){
		update();
		statusline();
		print();
		int c = getch();
		input(c);

	}
}

void Editor::update(){
	switch(mode){
		case 27:
		case 'n':
			status = "NORMAL";
			break;
		case 'i':
			status = "INSERT";
			break;
		case 'q':
			break;
	}
	section = " COLS: " + std::to_string(x) + " | ROWS: " + std::to_string(y) + " | FILE: " + filename;
	status.insert(0, " ");
}


void Editor::statusline(){
	start_color();
	if (mode  == 'n'){
		init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
	}else{
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
	} // This conditional change the background color according to the mode 
	attron(A_REVERSE); // Show background color on message
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	for (int i{}; i < COLS; ++i){
		mvprintw(LINES - 1, i, " ");
	}
	mvprintw(LINES - 1, 0, status.c_str()); // Print status on the bottom of the terminal
	mvprintw(LINES - 1, COLS - section.length(), section.c_str());
	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);
	attroff(A_REVERSE); // Turn off reverse mode
}


void Editor::input(int c){

	switch(c){
		case KEY_UP:
			up();
			return;

		case KEY_LEFT:
			left();
			return;

		case KEY_RIGHT:
			right();
			return;

		case KEY_DOWN:
			down();
			return;

	}

	switch(mode){
		case 27:
		case 'n':
			switch(c){
				case 'q':
					mode = 'q';
					break;
				case 'i':
					mode = 'i';
					break;
				case 'w':
					mode = 'w';
					save();
					refresh();
					endwin();
					exit(0);
					break;
			}
		break;
		case 'i':
			switch(c){
				case 27:
					mode = 'n';
					break;
				case 127:
				case KEY_BACKSPACE:
					if (x == 0 && y > 0){
						x = lines[y-1].length();
						lines[y-1] += lines[y];
						m_remove(y);
						up();
					}else if(x > 0){
						lines[y].erase(--x, 1);
					}
					break;
				case KEY_DC:
					if (x == lines[y].length() && y != lines.size() - 1){
						lines[y] += lines[y + 1];
					}else{
						lines[y].erase(x, 1);
					}
					break;
				case KEY_ENTER:
				case 10:
					if (x < lines[y].length()){
						m_insert(lines[y].substr(x, lines[y].length() - x), y+1);
						lines[y].erase(x, lines[y].length() - x);
					}else{
						m_insert("", y+1);
					}
					x = 0;
					down();
					break;

				case KEY_BTAB:
				case KEY_CTAB:
				case KEY_STAB:
				case KEY_CATAB:
				case 9:
					lines[y].insert(x, 2, '  ');
					x+=4;
					break;
				default:
					lines[y].insert(x, 1, c);
					++x;
					//the default case takes the characters and puts on our lines' string
			}
	}

}

void Editor::print(){
	for(size_t i{}; i < (size_t)LINES - 1; ++i){
		if(i >= lines.size()){
			move(i, 0);
			clrtoeol();
		}else{
			mvprintw(i, 0, lines[i].c_str());
		}
		clrtoeol();
	}

	move(y,x);
}

std::string Editor::m_tabs(std::string& line){
	std::size_t tab = line.find('\t');
	return tab == line.npos ? line : m_tabs(line.replace(tab, 1, "	"));
}

void Editor::m_insert(std::string line, int number){
	// This function inserts the line on wanted position represented by number variable
	line = m_tabs(line);
	lines.insert(lines.begin() + number, line);

}
void Editor::m_append(std::string& line){
	line = m_tabs(line);
	lines.push_back(line);
}


void Editor::up(){
	if (y > 0){
		--y;
	}
	if (x >= lines[y].length()){
		x = lines[y].length();
	}

	move(y, x);

}

void Editor::left(){
	if (x > 0){
		--x;
	}
	move(y, x);

}

void Editor::right(){
	if (x <= COLS && x <= lines[y].length() - 1){
		++x;
	}
	move(y, x);

}

void Editor::down(){
	if (y < LINES && y < lines.size() - 1){
		++y;
	}

	if (x >= lines[y].length()){
		x = lines[y].length();
	}
	move(y, x);
}

void Editor::open(){
	if (std::filesystem::exists(filename)){
		std::ifstream ifile(filename);
		if (ifile.is_open()){
			while(!ifile.eof()){
				std::string buffer;
				std::getline(ifile, buffer);
				m_append(buffer);
			}
		}else{
			throw std::runtime_error("Cannot open file. Permission denied");
		}
	}else{
		std::string str {};
		m_append(str);
	}
}

void Editor::save(){
	std::ofstream ofile(filename);
	if(ofile.is_open()){
		for(size_t i{}; i < lines.size(); ++i){
			if (lines[i] != lines[lines.size()-1]){
				ofile << lines[i] << '\n';
			}else{
				ofile << lines[i];
			}
		}
		ofile.close();
	}else{
		refresh();
		endwin();
		throw std::runtime_error("Cannot save file. Permission denied");
	}
}
