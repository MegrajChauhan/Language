#include "help.h"

uint32_t murmurhash3(const char *key, size_t len, uint32_t seed)
{
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t hash = seed;

    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *)(key);
    const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);

    uint32_t k;
    for (int i = 0; i < nblocks; i++)
    {
        k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        hash ^= k;
        hash = (hash << r2) | (hash >> (32 - r2));
        hash = hash * m + n;
    }

    k = 0;
    switch (len & 3)
    {
    case 3:
        k ^= tail[2] << 16;
    case 2:
        k ^= tail[1] << 8;
    case 1:
        k ^= tail[0];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        hash ^= k;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

size_t hash_string(const char *key, size_t bucket_count)
{
    return murmurhash3(key, strlen(key), 0) % bucket_count;
}

bool get_filename_from_path(slice *path)
{
    // just get the filename and that is all
    // start from end and go back until the start or a separator
    /// NOTE: This method implies there cannot be multiple files sharing name, even across
    // multiple folders
    if (!path)
        return false;
    char *tmp = path->_e;
    while (tmp != path->_s && *tmp != SEPARATOR)
    {
        tmp--;
    }
    path->_s = (*tmp == SEPARATOR) ? ++tmp : tmp;
    return true;
}

char *tok_to_str(uint64_t tok)
{
    switch (tok)
    {
    case U8:
        return "u8";
    case U16:
        return "u16";
    case U32:
        return "u32";
    case U64:
        return "u64";
    case I8:
        return "i8";
    case I16:
        return "i16";
    case I32:
        return "i32";
    case I64:
        return "i64";
    case F32:
        return "f32";
    case F64:
        return "f64";
    case VOID:
        return "void";
    case BOOL:
        return "bool";
    case TRUE:
        return "true";
    case FALSE:
        return "false";
    case IF:
        return "if";
    case ELSE:
        return "else";
    case VAR:
        return "var";
    case CONST:
        return "const";
    case GLOBAL:
        return "global";
    case FN:
        return "fn";
    case ENUM:
        return "enum";
    case STRUCT:
        return "struct";
    case COLON:
        return ":";
    case SEMI_COLON:
        return ";";
    case ASSIGN:
        return "=";
    case OPEN_PAREN:
        return "(";
    case CLOSE_PAREN:
        return ")";
    case OPEN_CURLY:
        return "{";
    case CLOSE_CURLY:
        return "}";
    case OPEN_BIGBRAC:
        return "[";
    case CLOSE_BIGBRAC:
        return "]";
    case QUOTE:
        return "\"";
    case SINGLE_QUOTE:
        return "'";
    case ACCESS_DOT:
        return ".";
    case EQUALS:
        return "==";
    }
}