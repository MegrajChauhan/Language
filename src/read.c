#include "read.h"

static bool check_file_ext(char *path)
{
    size_t len = strlen(path);
    if (len < 2)
        return false;
    char *st = path - 2;
    return (strcmp(st, ".l") == 0);
}

file *file_read(char *path)
{
    if (!check_file_ext(path))
    {
        report("The given input file \"%s%s%s\" doesn't have a supported file extension.\n", BOLDWHITE, path, RESET);
        return NULL;
    }
    file *f = (file *)malloc(sizeof(file));
    if (!f)
    {
        report_internal_error("Failed to initialize reader.");
        return NULL;
    }
    FILE *fd = fopen(path, "r");
    if (!fd)
    {
        switch (errno)
        {
        case EISDIR:
            report("The given input file \"%s%s%s\" is actually a directory.\n", BOLDWHITE, path, RESET);
            break;
        default:
            report("The given input file \"%s%s%s\" doesn't exist.\n", BOLDWHITE, path, RESET);
        }
        free(f);
        return NULL;
    }
    fseek(fd, SEEK_END, SEEK_CUR);
    size_t file_len = ftell(fd);
    rewind(fd);
    char buf[file_len + 1];
    fread(buf, 1, file_len, fd);
    buf[file_len] = 0;

    if ((f->fdata = stream_create(1, file_len)) == NULL)
    {
        free(f);
        fclose(fd);
        return NULL;
    }
    stream_populate(f->fdata, buf, file_len); // no way for this to fail
    fclose(fd);
    return f;
}