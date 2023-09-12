// CPython extension for checking wether or not a point (x, y)
//  located within a polygon.
// source: https://wrfranklin.org/Research/Short_Notes/pnpoly.html
// with slight modification.

#include <python3.10/Python.h>


// Our Python binding to our C function
// This will take one and only one non-keyword argument
static PyObject* point_in_polygon(PyObject* self, PyObject* args)
{
    int n_vert;
    PyObject *polygon_x;
    PyObject *polygon_y;
    double test_x, test_y;
   /* Parse the input tuple */
    if (!PyArg_ParseTuple(args, "iOOdd", &n_vert, &polygon_x, &polygon_y, &test_x, &test_y))
        return NULL;
    PyObject *iterator_polygon_x = PyObject_GetIter(polygon_x);
    if (!iterator_polygon_x)
        return NULL;
    PyObject *iterator_polygon_y = PyObject_GetIter(polygon_y);
    if (!iterator_polygon_y)
        return NULL;

    double *poly_array_x = (double*)calloc(n_vert, sizeof(double));
    double *poly_array_y = (double*)calloc(n_vert, sizeof(double));

    for (int N=0; N<n_vert; ++N) {
        poly_array_x[N] = PyFloat_AsDouble(PyIter_Next(iterator_polygon_x));
        poly_array_y[N] = PyFloat_AsDouble(PyIter_Next(iterator_polygon_y));
    }

    int i, j, c = 0;
    for (i = 0, j = n_vert-1; i < n_vert; j = i++) {
        if ( ((poly_array_y[i]>test_y) != (poly_array_y[j]>test_y)) &&
    	     (test_x < (poly_array_x[j]-poly_array_x[i]) * (test_y-poly_array_y[i]) / (poly_array_y[j]-poly_array_y[i]) + poly_array_x[i]) )
            c = !c;
    }

    free(poly_array_x);
    free(poly_array_y);

    return c ? Py_True: Py_False;
}

// Our Module's Function Definition struct
// We require this `NULL` to signal the end of our method
// definition
static PyMethodDef pip_methods[] = {
    { "point_in_polygon", point_in_polygon, METH_VARARGS,
    "Checks if a point is located within a polygon.\n"
    "\n"
    "Parameters\n"
    "----------\n"
    "length : int\n"
    "    Number of vertices in the polygon.\n"
    "polygon_x : list\n"
    "    List of x coordinates of the polygon vertices. Must have length 'length'.\n"
    "polygon_y : list\n"
    "    As polygon_x, but for the y coordinates.\n"
    "test_x : float\n"
    "    X coordinate of the test point.\n"
    "test_y : float\n"
    "    Y coordinate of the test point.\n"
    "\nReturns\n"
    "-------\n"
    "is_contained : bool\n"
    "    True if the polygon contains the test point.\n"
    "\n"
    "Edge Cases\n"
    "----------\n"
    "    Points on a right or upwards facing edges are considered\n"
    "    outside the polygon, points on left or downards facing\n"
    "    edges are considered inside.\n"
    "    Vertices which are on the top-left \n"
    "\n" },
    { NULL, NULL, 0, NULL }
};
// Edge Cases
//     ----------
//     Left edge: INSIDE
//     Top edge: OUTSIDE
//     Right edge: OUTSIDE
//     Bottom edge: INSIDE
// Our Module Definition struct
static struct PyModuleDef pip_module = {
    PyModuleDef_HEAD_INIT,
    "point_in_polygon",
    "Test Module",
    -1,
    pip_methods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_point_in_polygon(void)
{
    return PyModule_Create(&pip_module);
}
