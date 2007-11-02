#ifndef config_h
#define config_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "element.h"
#include "gui.h"

int config_parse(gui *ui, const char *config_file);

#endif
