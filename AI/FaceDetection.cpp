#ifdef _DEBUG
#else
#include "Python.h"
#include <iostream>

using namespace std;

void faceDetection() {
	Py_Initialize();

	string path = "./FaceDetection";
	string chdir_cmd = string("sys.path.append(\"") + path + "\")";
	const char* cstr_cmd = chdir_cmd.c_str();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(cstr_cmd);

	PyObject* pModule = NULL;
	pModule = PyImport_ImportModule("is_my_face");
	if (!pModule) {
		PyErr_Print();
		printf("Cant open python file!\n");
		return;
	}

	Py_Finalize();
}
#endif // _DEBUG