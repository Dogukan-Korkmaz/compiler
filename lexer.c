// Ana Sözcüksel analiz bloğu.

#include "compiler.h"
#include <string.h>
#include "helpers/vector.h"
#include "helpers/buffer.h"
#include <assert.h>

#define LEX_GETC_IF(buffer, c, exp)     \
    for (c = peekc(); exp; c = peekc()) \
    {                                   \
        buffer_write(buffer, c);        \
        nextc();                        \
    }


/*String karakterlerin token hale gelmesini sağlayan fonksyon, iki argümanı var, biri string
karakter dizisinin başladığı diğeri bittiği zamanı algılamak için örn. "merhaba dünya" burada arüman tırnak işaretidir.*/
static struct token* token_make_string(char start_delim, char end_delim)
{
    struct buffer* buf = buffer_create();
    assert(nextc() == start_delim);
    char c = nextc();
    for(; c != end_delim && c != EOF; c=nextc())//Karakter limitin sonuna veya dosyanın sonuna gelene kadar sonraki karakteri okuaycaktır. 
    {
        if (c == '\\')
        {
            /*'\n' gibi kaçış işaretleri burada algılanacak.Buraya daha sonra geri döneceğim.!!!*/
            continue;
        }

        buffer_write(buf, c);
    }

    buffer_write(buf, 0x00);
    return token_create(&(struct token){.type=TOKEN_TYPE_STRING,.sval=buffer_ptr(buf)});  
}

struct token* read_next_token(); // lexer.c belgesinde ilk bu fonksyonun çalışmasını istiyoruz.

static struct lex_process* lex_process;
static struct token tmp_token; // Geçici tokenler için değişken tanımı.

static char peekc()
{
    return lex_process->function->peek_char(lex_process);
}

//Bir sonraki karaktere geçme fonksyonu.
static char nextc()
{
    char c = lex_process->function->next_char(lex_process);
    lex_process->pos.col += 1;
    if (c == '\n')
    {
        lex_process->pos.line +=1;
        lex_process->pos.col = 1;
    }

    return c;
}

static void pushc(char c)
{
    lex_process->function->push_char(lex_process, c);
}

static struct pos lex_file_position()
{
    return lex_process->pos;
}

struct token* token_create(struct token* _token)
{
    memcpy(&tmp_token, _token, sizeof(struct token));
    tmp_token.pos = lex_file_position();
    return &tmp_token;
}

static struct token* lexer_last_token()
{
    return vector_back_or_null(lex_process->token_vec);
}

/*Karakterler arasındaki boşluğu algılayan ve
bir sonraki karakterin okunmasını sağlayan fonksyon.*/
static struct token* handle_whitespace()
{
    struct token* last_token = lexer_last_token();
    if (last_token)
    {
        last_token->whitespace = true;
    }

    nextc();
    return read_next_token();
}

const char* read_number_str()
{
    const char* num = NULL;
    struct buffer* buffer = buffer_create();
    char c = peekc();
    LEX_GETC_IF(buffer, c, (c >= '0' && c <= '9'));

    buffer_write(buffer, 0x00);
    return buffer_ptr(buffer);
}

unsigned long long read_number()
{
    const char* s = read_number_str();
    return atoll(s);
}

/* Decimal(2134),hexadecimal(0x500),binary(0b1000) gibisayı değerlerini tanımlama
 sırasında birbirlerinden ayıracak fonksyon,"unsigned" tanımlanmamamış değişkenler için kullanılı
bize gelecek sayının tipini bilmediğimiz için kullanıyoruz.*/
struct token* token_make_number_for_value(unsigned long number)
{
    return token_create(&(struct token){.type=TOKEN_TYPE_NUMBER,.llnum=number});
}

struct token* token_make_number()
{
    return token_make_number_for_value(read_number());
}

// Bir sonraki token'i okunmasını sağlayacak fonksiyon.
struct token* read_next_token()
{
    struct token* token = NULL;
    char c = peekc();
    switch(c)
    {
        NUMERIC_CASE:
        token = token_make_number();
        break;


        case '"':
        token = token_make_string('"', '"');//String karakterler tırnak işareti ile algılanır.
        break;
        // Boşulukları görmezden geleceğiz.    
        case ' ': // Boşluk
        case '\t': // Sekme(tab) boşluğu
        token = handle_whitespace();
        break;

        // EOF işlevi, dosyanın sonuna ulaşılana kadar False döndürmesidir.
        case EOF:
            // Sözcüksel analiz işlemi bitti.
        break;

        // Derleyicimiz tarafından desteklenmeyen karakter okunduğunda çalışacak kısım.
        default:
            compiler_error(lex_process->compiler, "Unexpected token\n");

    }
    return token;
}
int lex(struct lex_process* process)
{
    process->current_expression_count = 0;
    process->parentheses_buffer = NULL;
    lex_process = process;
    process->pos.filename = process->compiler->cfile.abs_path;

    //Girilen c dosyasındaki bütün karakterleri okuaycak.
    struct token* token = read_next_token();
    while(token)
    {
        vector_push(process->token_vec, token);
        token = read_next_token();
    }


    return LEXICAL_ANALYSIS_ALL_OK;
}