#include "editor.hpp"

int main(int argc, char **argv){
	if (argc > 1){
		auto editor = std::make_shared<Editor>(argv[1]);
		editor->run();
	}else{
		auto editor = std::make_shared<Editor>("");
		editor->run();
	}

	return 0;

}
