#pragma once

#include <stdarg.h>
#include <stdio.h>

int  MsgRet   (int return_value, const char* format, ...);
void MsgNoRet (const char* format, ...);

decltype(nullptr) LogMsgNullRet(const char* format, ...);

#define func_message(...)  {                              \
                            printf("\t%s: ", __func__);   \
                            printf(__VA_ARGS__);          \
                            } 

int SYSTEM(const char* format, ...);

// #pragma GCC diagnostic ignored 