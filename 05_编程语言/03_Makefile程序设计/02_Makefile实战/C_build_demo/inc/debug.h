#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <function.h>
#include <time.h>

#define DebugPrint(format, ...) printf("[INFO] [%s] [%d]" format "",__FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif