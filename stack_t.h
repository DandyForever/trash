#pragma once

#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <ctype.h>
#include <cmath>

typedef int StackElem_t;

const int Min_size_st = 1024, Add_size_st = 1024;
const int Crash_can1 = 666, Crash_can2 = 999;

class Stack_t
{
    private:
        int canaryleft;
        StackElem_t* data;
        int count_st;
        int size_st;
        int _hash;
        int _hash_data;
        int canaryright;

        void dump (const int value, FILE* file_error_info);
        void dump (const double value, FILE* file_error_info);
        void dump (const void* value, FILE* file_error_info);
        void StackDump (const Stack_t* stck);
        int Stack_hash_val ();
        int Stack_hash_data ();
        bool StackOK (const Stack_t* stck);
        void StackGrow ();

    public:
        Stack_t ();
        ~Stack_t ();
        void StackPush (const StackElem_t value);
        StackElem_t StackPop ();
        bool StackEmpty ();
        int StackSize ();
};
