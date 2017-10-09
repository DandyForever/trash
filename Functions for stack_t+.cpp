///Functions for stack
///-------------------
#include "stack_t.h"

#define assert(condition)                                                   \
        if (!condition)                                                     \
        {                                                                   \
        printf("ASSERTION %s in %s (%d)\n", #condition, __FILE__, __LINE__);\
        abort();                                                            \
        }

//--------------------------------------------------------
//! Macro to choose DEBUG mode
//!
//! @note write '1' to 'if' to use DEBUG mode
//! @note write '0' to 'if' not to use DEBUG mode
//--------------------------------------------------------
#if (1)
    #define ASSERT_STACK(st); \
            if (!StackOK (st))\
            {                 \
            StackDump(st);    \
            assert(0);        \
            }
#else
    #define ASSERT_STACK(s);
#endif

//---------------------------------------------------
//! Calculate hash of Stack variable
//!
//! @return hash of Stack variable
//!
//! @note Private Method of class Stack_t
//----------------------------------------------------
int Stack_t::Stack_hash_val ()
{
    return  (canaryleft) * (int)(data) + (int)(data)
            * (count_st) + (count_st) * (size_st)
            + (size_st) * (canaryright);
}

//----------------------------------------------------
//! Calculate hash of Stack's data
//!
//! @return hash of Stack's data
//!
//! @note Private Method of class Stack_t
//----------------------------------------------------
int Stack_t::Stack_hash_data ()
{
    int _hash_d = 0;
    for (int i = 0; i < count_st - 1; i++){
        _hash_d += data[i] * data[i + 1];
    }

    return _hash_d;
}

//---------------------------------------------------
//! Constructor of Stack variable
//!
//! @note Public Method of class Stack_t
//! @note fill Stack's parameters with start values
//----------------------------------------------------
Stack_t::Stack_t ()
{
    canaryleft = Crash_can1;
    canaryright = Crash_can2;

    size_st = Min_size_st;
    count_st = 1;

    data = (StackElem_t*) calloc (size_st, sizeof (StackElem_t));
    data[0] = Crash_can_dat1;
    data[size_st - 1] = Crash_can_dat2;

    _hash = this->Stack_hash_val ();
    _hash_data = this->Stack_hash_data ();

    ASSERT_STACK(this);
};

//---------------------------------------------------------
//! Destructor of Stack variable
//!
//! @note Public Method of class Stack_t
//! @note clear Stack and fill parameters with start values
//---------------------------------------------------------
Stack_t::~Stack_t ()
{
    ASSERT_STACK(this);

    free (data);
}

//----------------------------------------------------------
//! Dumper for int values
//!
//! @param [in] value             int value
//! @param [in] file_error_info   pointer on a file variable
//!
//! @note Private Method of class Stack_t
//-----------------------------------------------------------
void Stack_t::dump (const int value, FILE* file_error_info)
{
    fprintf (file_error_info, "%d\n", value);
}

//-----------------------------------------------------------
//! Dumper for double values
//!
//! @param [in] value             double value
//! @param [in] file_error_info   pointer on a file variable
//!
//! @note Private Method of class Stack_t
//------------------------------------------------------------
void Stack_t::dump (const double value, FILE* file_error_info)
{
    fprintf (file_error_info, "%f", value);
    if (value == NAN)
        fprintf (file_error_info, "       // !!! Not a Number !!!\n");
    else
        fprintf (file_error_info, "\n");
}

//-----------------------------------------------------------
//! Dumper for pointer values
//!
//! @param [in] value             pointer value
//! @param [in] file_error_info   pointer on a file variable
//!
//! @note Private Method of class Stack_t
//------------------------------------------------------------
void Stack_t::dump (const void* value, FILE* file_error_info)
{
    StackElem_t* val = (StackElem_t*) value;
    if (val == NULL)
        fprintf (file_error_info, "      // !!! ZERO POINTER !!!");
    else
        fprintf (file_error_info, "\n");
}

//---------------------------------------------------------
//! Dumper
//!
//! @param [in] stck             pointer on Stack variable
//!
//! @note Private Method of class Stack_t
//---------------------------------------------------------
void Stack_t::StackDump (const Stack_t* stck)
{
    FILE* file_error_info = fopen ("stack_error.txt", "w");
    assert (file_error_info);

    int hash_st = Stack_hash_val ();
    int hash_dat = Stack_hash_data ();

    if (hash_st != _hash)
        fprintf (file_error_info, "Hash of STACK is WRONG!!! [%d] but expected [%d]\n", hash_st, _hash);
    else
        fprintf (file_error_info, "Hash of STACK is OK [%d]\n", _hash);

    if (canaryleft == Crash_can1)
        fprintf(file_error_info, "LEFT canary is OK [%d]\n", canaryleft);
    else
        fprintf(file_error_info, "LEFT canary FAILED! [%d] but expected [%d]\n", canaryleft, Crash_can1);

    if (canaryright == Crash_can2)
        fprintf(file_error_info, "RIGHT canary is OK [%d]\n", canaryright);
    else
        fprintf(file_error_info, "RIGHT canary FAILED! [%d] but expected [%d]\n", canaryright, Crash_can2);

    fprintf (file_error_info, "Stack_t    [%p]   ERROR!!! {\n", stck);

    fprintf (file_error_info, "    size_st = %d", size_st);
    if (size_st < Min_size_st)
        fprintf (file_error_info, "   // !!! < Min size_st !!!\n");
    else
        fprintf (file_error_info, "\n");

    fprintf (file_error_info, "    count_st = %d", count_st);
    if (count_st < 0)
        fprintf (file_error_info, "   // !!! < 0 !!!");
    if (count_st >= size_st)
        fprintf (file_error_info, "   // !!! count_st >= size_st !!!\n");
    else
        fprintf (file_error_info, "\n");

    if (hash_dat - _hash_data < EPS)
        fprintf (file_error_info, "    Hash of stack's DATA is WRONG!!! [%d] but expected [%d]\n", hash_dat, _hash_data);
    else
        fprintf (file_error_info, "    Hash of stack's DATA is OK [%d]\n", hash_dat);

    if (data[0] - Crash_can_dat1 < EPS)
        fprintf (file_error_info, "    LEFT data canary is WRONG!!!\n");
    else
        fprintf (file_error_info, "    LEFT data canary is OK\n");
    if (data[size_st - 1] != Crash_can_dat2)
        fprintf (file_error_info, "    RIGHT data canary is WRONG!!!\n");
    else
        fprintf (file_error_info, "    RIGHT data canary is OK\n");

    fprintf (file_error_info, "    data [%p] = {\n", data);

    for (int i = 0; i < count_st; i++)
    {
        fprintf(file_error_info, "[%d] = ", i);
        dump(data[i], file_error_info);
    }
    fprintf (file_error_info, "   }\n"
                            "}\n");

    fclose (file_error_info);
}

/*FILE* openlog ()
{
    FILE* log = fopen("StackLog.txt", "w");
    return log;
}

void closelog (void)
{
    fclose(LogFILE);
}*/

//--------------------------------------------------------
//! Checker of stack
//!
//! @param [in] stck             pointer on Stack variable
//!
//! @return 1 if stack if OK and 0 if not
//!
//! @note Private Method of class Stack_t
//--------------------------------------------------------
bool Stack_t::StackOK (const Stack_t* stck)
{
    int hash_st = Stack_hash_val ();
    int hash_dat = Stack_hash_data ();

    return stck && count_st >= 0 && data && size_st > count_st
            && canaryleft == Crash_can1 && canaryright == Crash_can2
            && _hash == hash_st && _hash_data == hash_dat
            && data[size_st - 1] - Crash_can_dat2 < EPS && data[0] - Crash_can_dat1 < EPS;
}

//--------------------------------------------------------
//! Increase the size of stack
//!
//! @note Private Method of class Stack_t
//! @note Increase size of stack by Add_size_st
//--------------------------------------------------------
void Stack_t::StackGrow ()
{
    ASSERT_STACK(this);

    const int buffer_size_st = size_st + Add_size_st;
    assert(buffer_size_st > size_st);

    StackElem_t* buffer = (StackElem_t*) calloc (buffer_size_st, sizeof (StackElem_t));
    assert (buffer);

    memcpy (buffer, data, size_st * sizeof (StackElem_t));

    size_st += Add_size_st;
    free (data);
    data = buffer;
    data[size_st - 1] = Crash_can_dat2;

    _hash_data = this->Stack_hash_data();
    _hash = this->Stack_hash_val();

    ASSERT_STACK(this);
}

//--------------------------------------------------------
//! Put the value to the end of stack
//!
//! @param [in] value       value to put into stack
//!
//! @note Public Method of class Stack_t
//--------------------------------------------------------
void Stack_t::StackPush (const StackElem_t value)
{
    ASSERT_STACK(this);

    if (count_st + 1 == size_st - 1) StackGrow ();

    ASSERT_STACK(this);

    data[count_st++] = value;
    _hash_data = this->Stack_hash_data();
    _hash = this->Stack_hash_val();

    ASSERT_STACK(this);
}

//--------------------------------------------------------
//! Take the value from the top of stack
//!
//! @return value if Stack element from the top
//!
//! @note Public Method of class Stack_t
//--------------------------------------------------------
StackElem_t Stack_t::StackPop ()
{
    if (count_st < 1)
    {
        count_st--;

        if (0) ASSERT_STACK(this);
    }

    const StackElem_t pop_val = data[--count_st];
    _hash_data = this->Stack_hash_data();
    _hash = this->Stack_hash_val();

    ASSERT_STACK(this);

    return pop_val;
}

//--------------------------------------------------------
//! Check if the stack is empty
//!
//! @return 1 if stack if EMPTY and 0 if not
//!
//! @note Public Method of class Stack_t
//--------------------------------------------------------
bool Stack_t::StackEmpty ()
{
    ASSERT_STACK(this);

    return count_st == 1;
}

//--------------------------------------------------------
//! Give the number of elements in stack
//!
//! @return the number of stack elements
//!
//! @note Public Method of class Stack_t
//--------------------------------------------------------
int Stack_t::StackSize ()
{
    ASSERT_STACK(this);

    return count_st - 1;
}
