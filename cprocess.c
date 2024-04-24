#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
struct compile_process* compile_process_create(const char* filename, const char* filename_out, int flags)
{
    /*Verilen dosyanın okuma ve yazma işlemlerinde meydana gelecek hataların engellenmesi.*/
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        return NULL;
    }

    FILE* out_file = NULL;
    if (filename_out)
    {
        filename_out = fopen(filename_out, "w");
        if (!out_file)
        {
            return NULL;
        }
    }

    struct compile_process* process = calloc(1, sizeof(struct compile_process));
    process->flags = flags;
    process->cfile.fp = file;
    process->ofile = out_file;
    return process;
}