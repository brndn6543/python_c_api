# Calling Python From C
This demonstrates how to use the Python C API to enable C programs to initialize
the Python interpreter, run Python scripts, and access Python functions and variables
directly from a C program.

## Python.h
It is vital that you have the **Python development headers and libraries**. 
`Python.h` does not come by default with the standard Python installation.
Run the following, based on your platform, to install it.

### Debian
```c++
sudo apt-get install python3-dev
```

### Fedora
```c++
sudo dnf install python3-devel
```

### CentOS/RHEL
```c++
sudo yum install python3-devel
```

### Arch
```c++
sudo pacman -S python
```

### macOS w/Homebrew
```c++
brew install python
```

### Windows
If you are using the official installer, select the option to install the
**Python development headers and libraries**. You may confirm your installation directory
by using the following command:
```c++
py -3 -c "import sys; print(sys.prefix)"
```

## CMake
The CMake configuration is the most vital part of the setup.
```cmake
cmake_minimum_required(VERSION 3.15)
project(call_python C)

set(CMAKE_C_STANDARD 11)

# Find Python using your virtual environment explicitly
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Include Python headers
include_directories(${Python3_INCLUDE_DIRS})

# Add executable.
add_executable(call_python main.c)

# Link Python libraries to your executable
target_link_libraries(call_python ${Python3_LIBRARIES})
```

## Code
```c++
PyRun_SimpleString(
    "import sys\n"
    "sys.path.append('C:/Users/XXXX/XXXXX/XXXXXXXX/XXXX/python-c-api')\n"
);
```
This directly run Python code from a string within the embedded Python interpreter. By default,
Python searches the directory of the executable. If your script is elsewhere, you have to tell
Python where to look. This is a clean way to ensure Python finds your scripts without changing
working directories.
```c++
PyObject *pName = PyUnicode_FromString("py_script");
```
This creates a Python Unicode string object from a regular C string `py_script`.
This is needed because Python APIs accept arguments as Python object pointers `PyObject *`.
Python module names passed into Python's import functions must be Python string objects, not
simple C strings. This essentially prepares your C strings for the Python API that expects
Unicode strings.
<br>
<br>
```c++
Py_DECREF(pName);
```
This manually decreases the reference count for the Python object pointed to by `pName`.
Python manages memory via reference counting: every object has a counter. When the reference count
hits zero, Python knows the object can safely be deleted from memory.

`Py_DECREF()` explicitly tells Python the object is no longer needed, allowing clean-up. It avoids
memory leads by releasing Python objects that are no longer needed.
<br>
<br>
```c++
if (pFunc && PyCallable_Check(pFunc))
```
`PyCallable_Check()` verifies that the object, `pFunc`, is callable. We get `pFunc` from 
`PyObject_GetAttrString()`, which retrieves attributes from Python module or objects. This ensures
that a call is not attempted on something that is not callable.
<br>
<br>
```c++
PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString("Brandon"));
```
This creates a Python tuple containing arguments to pass to the Python function. Python functions
receive arguments as tuples internally. `PyTuple_Pack()` bundles your arguments into a single
Python tuple objects.

- `1` is the number of arguments you are passing.
- `PyUnicode_FromString("Brandon")` creates the argument as a Python string objects.

This is equivalent to calling a Python function with a single argument passed.
<br>
<br>
```c++
printf("[+] Python function return: %s\n", PyUnicode_AsUTF8(pValue));
```
Converts a Python Unicode string object `pValue` into a UTF-8 encoded C string. This is necessary
to translate Python strings back into a form usable by C.
<br>
<br>
```c++
Py_Finalize();
```
Terminates and cleans up the Python interpreter initialized at the beginning of the application `Py_Initialize()`.
It frees all memory and resources Python used during execution and ensures a clean shutdown. It prevents
memory leaks and resource locks.
<br>
<br>
| Call                   | Action                            | Purpose                                  |
| ---------------------- |-----------------------------------|------------------------------------------|
| `PyRun_SimpleString`   | Execute raw Python code from C.   | Configure Python environment dynamically |
| `PyUnicode_FromString` | Create Python Unicode strings.    | Convert from C strings to Python strings |
| `Py_DECREF`            | Decrease Python object ref-count. | Memory management (avoids leaks)         |
| `PyCallable_Check`     | Verify Python object is callable. | Safety check before calling functions    |
| `PyTuple_Pack`         | Create a tuple of Python args.    | Passing arguments to Python functions    |
| `PyUnicode_AsUTF8`     | Convert Python strings back to C. | Retrieve Python function return values   |
| `Py_Finalize`          | Shut down Python interpreter.     | Clean shutdown of embedded interpreter   |
