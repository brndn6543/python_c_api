#include <stdio.h>
#include <Python.h>

int main(void) {
	// Initialize the interpreter.
	Py_Initialize();

	// Adjust Python sys.path
	PyRun_SimpleString(
		"import sys\n"
		"sys.path.append('C:/Users/Path/To/File/python-c-api')\n"
	);

	// Set the name of the script.
	PyObject *pName = PyUnicode_FromString("py_script");

	// Import the script as a Python module.
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		// Get the reference to the Python function.
		PyObject *pFunc = PyObject_GetAttrString(pModule, "greet");

		if (pFunc && PyCallable_Check(pFunc)) {
			// Prepare arguments for the Python function.
			PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString("Brandon"));

			// Call the function.
			PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if (pValue != NULL) {
				// Print the return value.
				printf("[+] Python function return: %s\n", PyUnicode_AsUTF8(pValue));
				Py_DECREF(pValue);
			} else {
				PyErr_Print();
				fprintf(stderr, "[-] Python function return: %s\n", PyUnicode_AsUTF8(pValue));
			}
			Py_DECREF(pFunc);
		} else {
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
			fprintf(stderr, "[-] Cannot find function 'greet'\n");
		}
		Py_DECREF(pModule);
	} else {
		PyErr_Print();
		fprintf(stderr, "[-] Failed to load module 'greet'\n");
		return 1;
	}

	// Clean up and exit the interpreter.
	Py_Finalize();
	return 0;
}