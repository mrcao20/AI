#include "AI.h"
#include "SingleApplication.h"
#include "MyNativeEventFilter.h"

int main(int argc, char *argv[]){
	SingleApplication a(argc, argv);
	if (a.isRunning())
		return 0;
	AI w;
	a.w = &w;
	w.show();
	MyNativeEventFilter nativeEventFilter;
	nativeEventFilter.setMainWidget(&w);
	a.installNativeEventFilter(&nativeEventFilter);
	return a.exec();
}
