#include "color.h"

struct opts {
    int inertia;
    struct color bg;
};

struct opts parse_opts(int argc, char *argv[]);