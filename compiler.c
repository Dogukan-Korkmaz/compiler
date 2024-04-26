/*Ana compiler bloğu*/
#include "compiler.h"
/*"Char*"dan sonra gelen 
"const" anahtar sözcüğü, işaretçinin 
işaret ettiği karakterin de sabit olduğu ve 
değiştirilemeyeceği anlamına gelir.*/

struct lex_process_functions compile_lex_functions = {
    .next_char = compile_process_char,
    .peek_char = compile_peek_char,
    .push_char = compile_push_char,
};

int compile_file(const char* filename, const char* out_filename, int flags)
{
    struct compile_process* process = compile_process_create(filename, out_filename, flags);
    if (!process)
        return COMPILER_FAILED_WITH_ERRORS;
    
    // Lexical analiz
    struct lex_process* lex_process = lex_process_create(process, &compiler_lex_functions, NULL);
    if(!lex_process)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    if (lex(lex_process) != LEXICAL_ANALYSIS_ALL_OK)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    // Parsing

    // Kod oluşturma işlemini
    
    return COMPILER_FAILED_WITH_ERRORS; 
}