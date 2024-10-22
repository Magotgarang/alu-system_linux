#include <Python.h>
#include <object.h>
#include <listobject.h>
#include <bytesobject.h>

/**
 * print_python_list - Prints basic information about Python lists
 * @p: Pointer to the PyObject (Python list object)
 */
void print_python_list(PyObject *p)
{
    Py_ssize_t size, i;
    Py_ssize_t allocated;
    const char *type;

    size = PyList_Size(p);
    allocated = ((PyListObject *)p)->allocated;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", allocated);

    for (i = 0; i < size; i++)
    {
        type = (Py_TYPE(PyList_GetItem(p, i)))->tp_name;
        printf("Element %zd: %s\n", i, type);
    }
}

/**
 * print_python_bytes - Prints basic information about Python bytes objects
 * @p: Pointer to the PyObject (Python bytes object)
 */
void print_python_bytes(PyObject *p)
{
    Py_ssize_t size;
    char *string;
    int i;

    printf("[.] bytes object info\n");
    if (!PyBytes_Check(p))
    {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }

    size = PyBytes_Size(p);
    string = PyBytes_AsString(p);

    printf("  size: %zd\n", size);
    printf("  trying string: %s\n", string);
    printf("  first %zd bytes: ", size + 1 > 10 ? 10 : size + 1);

    for (i = 0; i < size + 1 && i < 10; i++)
    {
        printf("%02x", string[i] & 0xff);
        if (i + 1 < size + 1 && i + 1 < 10)
            printf(" ");
    }
    printf("\n");
}

