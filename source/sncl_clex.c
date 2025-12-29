#include <sncl_clex.h>

#include <stdlib.h>

int sncl_is_whitespace(char c);

int sncl_token(sncl_lex_t *lexer, long token, char *start, char *end);
int sncl_eof(sncl_lex_t *lexer);
int sncl_parse_string(sncl_lex_t *lexer, char *p, long token);
int sncl_parse_integer_suffixes(sncl_lex_t *lexer, long token, char *p, char *q);
int sncl_parse_float_suffixes(sncl_lex_t *lexer, long token, char *p, char *q);

int sncl_ishex(int c);
int sncl_utf8_encode(char *out, unsigned int codepoint);

const char *sncl_find_char(const char *str, int ch);

void sncl_init_lexer(sncl_lex_t *lexer, const char *stream, const char *stream_end, char *string_store,
                     int store_length) {
    lexer->start = stream;
    lexer->end = stream_end;
    lexer->point = (char *)lexer->start;
    lexer->str_storage.ptr = string_store;
    lexer->str_storage.len = store_length;
    lexer->token = 0;
}

int sncl_clex_get_token(sncl_lex_t *lexer) {
    char *p = lexer->point;

    for (;;) {
        // whitespace
        while (p != lexer->end && sncl_is_whitespace(*p))
            ++p;

        // comments
        if (p != lexer->end && p[0] == '/' && p[1] == '/') {
            while (p != lexer->end && *p != '\r' && *p != '\n')
                ++p;
            continue;
        }

        if (p != lexer->end && p[0] == '/' && p[1] == '*') {
            char *begin = p;
            p += 2;
            while (p != lexer->end && (p[0] != '*' || p[1] != '/'))
                ++p;
            if (p == lexer->end)
                return sncl_token(lexer, CLEX_ERROR, begin, p - 1);
            p += 2;
            continue;
        }
    }

    if (p == lexer->end) {
        return sncl_eof(lexer);
    }

    switch (*p) {
    case '+':
        if (p + 1 != lexer->end) {
            if (p[1] == '+')
                return sncl_token(lexer, CLEX_PLUSPLUS, p, p + 1);
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_PLUSEQU, p, p + 1);
        }
        break;
    case '-':
        if (p + 1 != lexer->end) {
            if (p[1] == '-')
                return sncl_token(lexer, CLEX_MINUSMINUS, p, p + 1);
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_MINUSEQU, p, p + 1);
            if (p[1] == '>')
                return sncl_token(lexer, CLEX_ARROW, p, p + 1);
        }
        break;
    case '&':
        if (p + 1 != lexer->end) {
            if (p[1] == '&')
                return sncl_token(lexer, CLEX_ANDAND, p, p + 1);
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_ANDEQU, p, p + 1);
        }
        break;
    case '|':
        if (p + 1 != lexer->end) {
            if (p[1] == '|')
                return sncl_token(lexer, CLEX_OROR, p, p + 1);
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_OREQU, p, p + 1);
        }
        break;
    case '=':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_EQUAL, p, p + 1);
            if (p[1] == '>')
                return sncl_token(lexer, CLEX_EQUARROW, p, p + 1);
        }
        break;
    case '!':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_NEQUAL, p, p + 1);
        }
        break;
    case '^':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_XOREQU, p, p + 1);
        }
        break;
    case '%':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_MODEQU, p, p + 1);
        }
        break;
    case '*':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_MULEQU, p, p + 1);
        }
        break;
    case '/':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_DIVEQU, p, p + 1);
        }
        break;
    case '<':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_LEQUAL, p, p + 1);
            if (p[1] == '<') {
                if (p + 2 != lexer->end && p[2] == '=')
                    return sncl_token(lexer, CLEX_SHLEQU, p, p + 2);
                return sncl_token(lexer, CLEX_SHL, p, p + 1);
            }
        }
        break;
    case '>':
        if (p + 1 != lexer->end) {
            if (p[1] == '=')
                return sncl_token(lexer, CLEX_GEQUAL, p, p + 1);
            if (p[1] == '>') {
                if (p + 2 != lexer->end && p[2] == '=')
                    return sncl_token(lexer, CLEX_SHREQU, p, p + 2);
                return sncl_token(lexer, CLEX_SHR, p, p + 1);
            }
        }
        break;
    case '"':
        return sncl_parse_string(lexer, p, CLEX_DSTRING);
    case '\'':
        return sncl_parse_string(lexer, p, CLEX_SSTRING);
    case '0':
        if (p + 1 != lexer->end) {
            if (p[1] == 'x' || p[1] == 'X') {
                char *q;
                lexer->int_num = strtol(p, &q, 16);

                if (q == p + 2)
                    sncl_token(lexer, CLEX_ERROR, p - 2, p - 1);
                return sncl_parse_integer_suffixes(lexer, CLEX_INTEGER, p, q);
            }

            if (p[1] == 'b') {
                char *q;
                lexer->int_num = strtol(p, &q, 2);
                if (q == p + 2)
                    sncl_token(lexer, CLEX_ERROR, p - 2, p - 1);
                return sncl_parse_integer_suffixes(lexer, CLEX_INTEGER, p, q);
            }
        }
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        // clang-format off
        {
            char *q = p;
            while (q != lexer->end && (*q >= '0' && *q <= '9'))
                ++q;
            if (q != lexer->end) {
                if (*q == '.') {
                    lexer->real_num = strtod(p, &q);

                    return sncl_parse_float_suffixes(lexer, CLEX_DOUBLE, p, q);
                }
            }
        }
        // clang-format on

        if (p[0] == '0') {
            char *q = p;
            lexer->int_num = strtol(p, &q, 8);
            return sncl_parse_integer_suffixes(lexer, CLEX_INTEGER, p, q);
        }

        {
            char *q = p;
            lexer->int_num = strtol(p, &q, 10);
            return sncl_parse_integer_suffixes(lexer, CLEX_INTEGER, p, q);
        }

        break;
    default:
        if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' || *p == '$') {
            int n = 0;
            lexer->str.ptr = lexer->str_storage.ptr;
            do {
                if (n + 1 >= lexer->str_storage.len)
                    return sncl_token(lexer, CLEX_ERROR, p, p + n);
                lexer->str.ptr[n] = p[n];
                ++n;
            } while ((p[n] >= 'a' && p[n] <= 'z') || (p[n] >= 'A' && p[n] <= 'Z') || (p[n] >= '0' && p[n] <= '9') ||
                     p[n] == '_' || p[n] == '$' || (unsigned char)p[n] >= 128);
            lexer->str.ptr[n] = 0;
            lexer->str.len = n;
            return sncl_token(lexer, CLEX_IDENTI, p, p + n - 1);
        }

        if (*p == 0) {
            return sncl_token(lexer, CLEX_EOF, p, p);
        }

        break;
    }

    return sncl_token(lexer, *p, p, p);
}

void sncl_clex_get_location(const sncl_lex_t *lexer, const char *point, sncl_lex_loc_t *location) {
    const char *p = lexer->start;
    int line = 1;
    int col = 0;

    while (*p && p < point) {
        if (*p == '\n' || *p == '\r') {
            p += (p[0] + p[1] == '\r' + '\n' ? 2 : 1);
            line++;
            col = 0;
        } else {
            ++p;
            ++col;
        }
    }

    location->line = line;
    location->column = col;
}

int sncl_is_whitespace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f'; }

int sncl_token(sncl_lex_t *lexer, long token, char *start, char *end) {
    lexer->token = token;
    lexer->error.start = start;
    lexer->error.end = end;
    lexer->point = end + 1;
    return 1;
}

int sncl_eof(sncl_lex_t *lexer) {
    lexer->token = CLEX_EOF;
    return 0;
}

int sncl_parse_string(sncl_lex_t *lexer, char *p, long token) {
    char *begin = p;
    char delim = *p++;
    char *out = lexer->str_storage.ptr;
    char *out_end = lexer->str_storage.ptr + lexer->str_storage.len;

    while (*p != delim) {
        int n;
        if (*p == '\\') {
            switch (p[1]) {
            case '\\':
                n = '\\';
                break;
            case '\'':
                n = '\'';
                break;
            case '"':
                n = '"';
                break;
            case 't':
                n = '\t';
                break;
            case 'f':
                n = '\f';
                break;
            case 'n':
                n = '\n';
                break;
            case 'r':
                n = '\r';
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7': {
                int value = 0;
                int count = 0;

                // up to 3 octal digits
                while (count < 3 && p[1] >= '0' && p[1] <= '7') {
                    value = (value << 3) | (p[1] - '0');
                    p++;
                    count++;
                }

                n = value & 0xFF;
                break;
            }
            case 'x':
            case 'X': {
                int value = 0;
                int h;

                p += 2; // skip \x

                if ((h = sncl_ishex(*p)) < 0)
                    return sncl_token(lexer, CLEX_ERROR, begin, p);

                while ((h = sncl_ishex(*p)) >= 0) {
                    value = (value << 4) | h;
                    p++;
                }

                n = value & 0xFF;
                continue;
            }
            case 'u':
            case 'U': {
                int digits = (p[1] == 'u') ? 4 : 8;
                unsigned int value = 0;

                p += 2;

                for (int i = 0; i < digits; i++) {
                    int h = sncl_ishex(*p++);
                    if (h < 0)
                        return sncl_token(lexer, CLEX_ERROR, begin, p);
                    value = (value << 4) | h;
                }

                int bytes = sncl_utf8_encode(out, value);
                if (bytes < 0 || out + bytes > out_end)
                    return sncl_token(lexer, CLEX_ERROR, begin, p);

                out += bytes;
                continue;
            }
            default:
                n = p[1];
                break;
            }

            p += 2;
            if (n < 0)
                return sncl_token(lexer, CLEX_ERROR, begin, p);
        } else {
            n = (int)(*p++);
        }

        if (out + 1 > out_end) {
            return sncl_token(lexer, CLEX_ERROR, begin, p);
        }

        *out++ = (char)n;
    }

    *out = 0;
    lexer->str.ptr = lexer->str_storage.ptr;
    lexer->str.len = (int)(out - lexer->str_storage.ptr);
    return sncl_token(lexer, token, begin, p);
}

int sncl_parse_integer_suffixes(sncl_lex_t *lexer, long token, char *p, char *q) {
    lexer->str.ptr = lexer->str_storage.ptr;
    lexer->str.len = 0;

    while ((*q >= 'a' && *q <= 'z') || (*q >= 'A' && *q <= 'Z')) {
        if (sncl_find_char("uUlL", *q) == 0)
            return sncl_token(lexer, CLEX_ERROR, p, q);
        if (lexer->str.len + 1 >= lexer->str_storage.len)
            return sncl_token(lexer, CLEX_ERROR, p, q);
        lexer->str.ptr[lexer->str.len++] = *q;
    }

    return sncl_token(lexer, token, p, q - 1);
}

int sncl_parse_float_suffixes(sncl_lex_t *lexer, long token, char *p, char *q) {
    lexer->str.ptr = lexer->str_storage.ptr;
    lexer->str.len = 0;

    while ((*q >= 'a' && *q <= 'z') || (*q >= 'A' && *q <= 'Z')) {
        if (sncl_find_char("fFlL", *q) == 0)
            return sncl_token(lexer, CLEX_ERROR, p, q);
        if (lexer->str.len + 1 >= lexer->str_storage.len)
            return sncl_token(lexer, CLEX_ERROR, p, q);
        lexer->str.ptr[lexer->str.len++] = *q;
    }

    return sncl_token(lexer, token, p, q - 1);
}

int sncl_ishex(int c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

int sncl_utf8_encode(char *out, unsigned int codepoint) {
    if (codepoint <= 0x7F) {
        out[0] = codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        out[0] = 0xC0 | (codepoint >> 6);
        out[1] = 0x80 | (codepoint & 0x3F);
        return 2;
    } else if (codepoint <= 0xFFFF) {
        out[0] = 0xE0 | (codepoint >> 12);
        out[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[2] = 0x80 | (codepoint & 0x3F);
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        out[0] = 0xF0 | (codepoint >> 18);
        out[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        out[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[3] = 0x80 | (codepoint & 0x3F);
        return 4;
    }
    return -1;
}

const char *sncl_find_char(const char *str, int ch) {
    for (; *str; ++str)
        if (*str == ch)
            return str;
    return 0;
}
