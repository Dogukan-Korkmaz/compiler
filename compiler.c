/*Ana compiler bloğu*/
#include "compiler.h"
#include <stdarg.h>//(...) sonsuz argüman kullanmak için ihtiyacımın olan kütüphane.
#include <stdlib.h>//exit() fonksyonu bu kütüphanede.
/*"Char*"dan sonra gelen 
"const" anahtar sözcüğü, işaretçinin 
işaret ettiği karakterin de sabit olduğu ve 
değiştirilemeyeceği anlamına gelir.*/

struct lex_process_functions compiler_lex_functions = {
    .next_char=compile_process_next_char,
    .peek_char=compile_process_peek_char,
    .push_char=compile_process_push_char
};

/*Desteklenmeyen karakter algılandığında..vb çalışacak fonksyon yani hatalar için fonksyon.
Sonsuz argüman alabilir "printf()" fonsyonu gibi bu yüzden "..." kullanıyoruz.*/
void compiler_error(struct compile_process* compiler, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, " on line %i, col %i in file %s\n", compiler->pos.line, compiler->pos.col, compiler->pos.filename);
    exit(-1);//"exit" fonksyonu programın çalışmasını sonlandırır.İçine "-1" yazılıyor olması çalışma sırasında bir hatanın meydana geldiğini gösterir.
}
/*Uyarı fonksyonu, kod düzgün çalışsada bazı durumlar için kullanıcıyı uyaracak olan geri bildirimi sağlayacak fonksyon.
çalışmayı durdurmaya gerek oladığı için exit() fonksyonunu kullanmaya gerek yok.*/
void compiler_warning(struct compile_process* compiler, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, " on line %i, col %i in file %s\n", compiler->pos.line, compiler->pos.col, compiler->pos.filename);
}

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