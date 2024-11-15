#include "stream.h"

stream *stream_create(size_t elem_len, size_t elem_count)
{
    stream *strm = (stream *)malloc(sizeof(stream));
    if (!strm)
    {
        report_internal_error("Failed to allocate memory for stream.");
        return NULL;
    }

    strm->buffer = malloc(elem_len * elem_count);
    if (!strm->buffer)
    {
        free(strm);
        report_internal_error("Failed to allocate memory for stream buffer.");
        return NULL;
    }

    strm->elem_len = elem_len;
    strm->elem_count = elem_count;
    strm->read_ptr = strm->write_ptr = 0;

    return strm;
}

void *stream_read(stream *strm)
{
    check_ptr(strm, buffer);

    if (strm->read_ptr >= strm->elem_count)
        return NULL; // No more elements to read

    void *elem = (char *)strm->buffer + strm->read_ptr * strm->elem_len;
    strm->read_ptr++;
    return elem;
}

bool stream_write(stream *strm, void *data)
{
    check_ptr(strm, buffer);

    if (strm->write_ptr >= strm->elem_count)
    {
        report_internal_error("Stream buffer overflow.");
        return false;
    }

    memcpy((char *)strm->buffer + strm->write_ptr * strm->elem_len, data, strm->elem_len);
    strm->write_ptr++;
    return true;
}

bool stream_populate(stream *strm, void *data, size_t nelem)
{
    check_ptr(strm, buffer);

    if (strm->write_ptr >= strm->elem_count || (strm->write_ptr + nelem) > strm->elem_count)
    {
        report_internal_error("Stream buffer overflow.");
        return false;
    }

    memcpy((char *)strm->buffer + strm->write_ptr * strm->elem_len, data, nelem * strm->elem_len);
    strm->write_ptr += nelem;
    return true;
}

void *stream_peek(stream *strm, size_t off)
{
    check_ptr(strm, buffer);
    if ((strm->read_ptr + off) > strm->elem_count)
        return NULL;
    return ((char *)strm->buffer + strm->elem_len * (strm->read_ptr + off));
}

void stream_reset(stream *strm)
{
    check_ptr(strm, buffer);
    strm->read_ptr = strm->write_ptr = 0;
}

bool stream_has_more(stream *strm)
{
    check_ptr(strm, buffer);
    return strm->read_ptr < strm->elem_count;
}

bool stream_resize(stream *strm, size_t new_elem_count)
{
    check_ptr(strm, buffer);

    void *new_buffer = realloc(strm->buffer, strm->elem_len * new_elem_count);
    if (!new_buffer)
    {
        report_internal_error("Failed to resize the stream buffer.");
        return false;
    }

    strm->buffer = new_buffer;
    strm->elem_count = new_elem_count;

    if (strm->read_ptr > new_elem_count)
        strm->read_ptr = new_elem_count;
    if (strm->write_ptr > new_elem_count)
        strm->write_ptr = new_elem_count;

    return true;
}

void stream_destroy(stream *strm)
{
    check_ptr(strm, buffer);
    free(strm->buffer);
    free(strm);
}

void *stream_at(stream *strm)
{
    check_ptr(strm, buffer);
    return (char *)strm->buffer + strm->elem_len * (strm->read_ptr - 1);
}