/*Ana compiler bloğu*/
#include "compiler.h"
/*"Char*"dan sonra gelen 
"const" anahtar sözcüğü, işaretçinin 
işaret ettiği karakterin de sabit olduğu ve 
değiştirilemeyeceği anlamına gelir.*/
int compile_file(const char* filename, const char* out_filename, int flags)
{
    struct compile_process* process = compile_process_create(filename, out_filename, flags);
    if (!process)
        return COMPILER_FAILED_WITH_ERRORS;
    
    // Lexical analiz

    // Parsing

    // Kod oluşturma işlemini
    
    return 0; 
}