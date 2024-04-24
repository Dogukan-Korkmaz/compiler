#ifndef PEACHCOMPILER_H
#define PEACHCOMPILER_H

#include <stdio.h>

// Hata mesajları burada tutuluyor.
enum
{
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS
};

struct compile_process
{
    /*Bu dosyanın nasıl derlenmesi gerektiğine ilişkin bayraklar.*/
    int flags;

    struct compile_process_input_file
    {
        FILE* fp;
        const char* abs_path;
    } cfile;
    
    FILE* ofile;
};



/*derleyici, derleyicimiz boyunca erişilmesi gereken işlevler için tüm prototipleri içerecektir.*/
int compile_file(const char* filename, const char* out_filename, int flags);
struct compile_process* compile_process_create(const char* filename, const char* filename_out, int flags);
#endif