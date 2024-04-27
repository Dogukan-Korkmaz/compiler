#ifndef PEACHCOMPILER_H
#define PEACHCOMPILER_H

#include <stdio.h>
#include <stdbool.h>

// Pozisyonu değerleri,token için(satır, sütun ve dosya).
struct pos
{
    int line;
    int col;
    const char* filename;
};

//Numara karakterlerinin tanımlanması.
#define NUMERIC_CASE \
    case '0':       \
    case '1':       \
    case '2':       \
    case '3':       \
    case '4':       \
    case '5':       \
    case '6':       \
    case '7':       \
    case '8':       \
    case '9'       

// Analiz işlemi sonrası geri bildirim için.  
enum
{
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_ERROR
};

// Token tipleri için.(yeni satır,sembol,string,yorum satırı...vb)
enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE
};


// Sözcüksel analiz[1] için token yapıları tanımlanması.
struct token
{
    int type;
    int flags;
    struct pos pos; // Token dosyanın neresinde olduğunu anlamak için değişken ataması.

    // union içine yazılan herşey aynı belleği paylaşır.
    union
    {
        char cval;
        const char* sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void* any;
    };

    // Eğer tokenler arası boşluk varsa True.
    bool whitespace;

    const char* between_brackets;
};

// Sözcükler arası geçiş için gerekli fonksyonlar.
struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process*  process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process*  process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process*  process, char c);
struct lex_process_functions
{
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

// Sözcüksel analiz işlemi.(Sözcük derken string anlamında değil her şey olabilir.Örn. 3, / ,  , ü)
struct lex_process
{
    struct pos pos;
    struct vector* token_vec;
    struct compile_process* compiler;

    // lex(sözcük) kaç satır arasında olduğunu tutan değişken.örn. ((50))
    int current_expression_count;
    struct buffer* parentheses_buffer;
    struct lex_process_functions* function;

    /*Derleyicinin anlayamadığı ama kullanıcının anlayabildiği veriler için.*/
    void* private;
};


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

    //Sözcüksel analiz[3], sözcüğün pozisyonunu(satır,sütun) öğrenmek için değişken tanımlanması. 
    struct pos pos;

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


char compile_process_next_char(struct lex_process* lex_process);
char compile_process_peek_char(struct lex_process* lex_process);
void compile_process_push_char(struct lex_process* lex_process, char c);


void compiler_error(struct compile_process* compiler, const char* msg, ...);
void compiler_warning(struct compile_process* compiler, const char* msg, ...);

struct lex_process* lex_process_create(struct compile_process* compile, struct lex_process_functions* functions, void* private);
void lex_process_free(struct lex_process* process);
void* lex_process_private(struct lex_process* process);
struct vector* lex_process_tokens(struct lex_process* process);
int lex(struct lex_process* process);

#endif