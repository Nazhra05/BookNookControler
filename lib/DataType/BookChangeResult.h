#ifndef BOOK_CHANGES_RESULT_H
#define BOOK_CHANGES_RESULT_H
#include <Arduino.h>
#include <DynamicArray.h>

struct BookChangeResult
{
    String status;
    DynamicArray<String> data;
};

#endif