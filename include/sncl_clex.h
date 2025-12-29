/* SNCL C Lexer v1.0
   Lexer for a C like language with some extensions, based on stb_c_lexer.
   Intended for use with the Sakura C compiler.

   Contributors:
   - StarIitNova (fynotix.dev@gmail.com)
 */

#ifndef SNCL_CLEX_H__
#define SNCL_CLEX_H__

typedef struct {
    const char *start;
    const char *end;
    char *point;

    struct {
        char *ptr;
        int len;
    } str_storage;

    struct {
        char *start;
        char *end;
    } error;

    long token;
    union {
        double real_num;
        int int_num;
        struct {
            char *ptr;
            int len;
        } str;
    };
} sncl_lex_t;

typedef struct {
    int line;
    int column;
} sncl_lex_loc_t;

enum {
    CLEX_EOF = 256,
    CLEX_ERROR,
    CLEX_INTEGER,
    CLEX_DOUBLE,
    CLEX_IDENTI,
    CLEX_DSTRING,
    CLEX_SSTRING,
    CLEX_CHARACT,
    CLEX_EQUAL,
    CLEX_NEQUAL,
    CLEX_LEQUAL,
    CLEX_GEQUAL,
    CLEX_ANDAND,
    CLEX_OROR,
    CLEX_SHL,
    CLEX_SHR,
    CLEX_PLUSPLUS,
    CLEX_MINUSMINUS,
    CLEX_PLUSEQU,
    CLEX_MINUSEQU,
    CLEX_MULEQU,
    CLEX_DIVEQU,
    CLEX_MODEQU,
    CLEX_ANDEQU,
    CLEX_OREQU,
    CLEX_XOREQU,
    CLEX_ARROW,
    CLEX_EQUARROW,
    CLEX_SHLEQU,
    CLEX_SHREQU,

    CLEX_LAST
};

void sncl_init_lexer(sncl_lex_t *lexer, const char *stream, const char *stream_end, char *string_store,
                     int store_length);

// for parsing
int sncl_clex_get_token(sncl_lex_t *lexer);

// for errors
void sncl_clex_get_location(const sncl_lex_t *lexer, const char *point, sncl_lex_loc_t *location);

#endif // SNCL_CLEX_H__
