#include "read.h"

static bool check_file_ext(char *path)
{
    size_t len = strlen(path);
    if (len < 2)
        return false;
    char *st = path + (len - 2);
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

    /**
     * Since fopen doesn't fail even if you trying opening a directory,
     * we need some form of sanity checks to verify the file.
     * If fopen works but IO fails then we may seek errno to know if it is a directory
     */
    if (fgetc(fd) == EOF)
    {
        switch (errno)
        {
        case EISDIR:
            report("The given input file \"%s%s%s\" is actually a directory.\n", BOLDWHITE, path, RESET);
            break;
        }
        free(f);
        fclose(fd);
        return NULL;
    }
    else
        rewind(fd); // success

    fseek(fd, SEEK_SET, SEEK_END);
    size_t file_len = ftell(fd);
    rewind(fd);
    char buf[file_len];
    fread(buf, 1, file_len, fd);

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

void file_close(file *f)
{
    check_ptr(f, fdata);
    stream_destroy(f->fdata);
    free(f);
}