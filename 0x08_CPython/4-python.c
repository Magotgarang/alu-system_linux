#define PY_SSIZE_T_CLEAN
#include <Python.h>

void print_python_string(PyObject *p) {
    // Check if the PyObject is a Unicode string
    if (!PyUnicode_Check(p)) {
        printf("[.] string object info\n");
        printf("  [ERROR] Invalid String Object\n");
        return;
    }

    // Get the length of the string
    Py_ssize_t length = PyUnicode_GetLength(p);
    
    // Convert the Unicode string to a UTF-8 encoded C string
    const char *value = PyUnicode_AsUTF8(p);
    
    // Determine the string type
    const char *type;
    if (PyUnicode_IS_COMPACT_ASCII(p)) {
        type = "compact ascii";
    } else if (PyUnicode_IS_COMPACT_UNICODE(p)) {
        type = "compact unicode object";
    } else {
        type = "non-compact unicode object";
    }

    // Print the information
    printf("[.] string object info\n");
    printf("  type: %s\n", type);
    printf("  length: %zd\n", length);
    printf("  value: %s\n", value ? value : "NULL");  // Check if value is NULL
}
