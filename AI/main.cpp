#include "AI.h"
#include "SingleApplication.h"

int main(int argc, char *argv[]){
	SingleApplication a(argc, argv);
	if (a.isRunning())
		return 0;
	AI w;
	a.w = &w;
	w.show();
	return a.exec();
}
