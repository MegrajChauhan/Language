#ifndef _STREAM_
#define _STREAM_

#include "utils.h"
#include "report.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct stream stream;

struct stream
{
    void *buffer;
    size_t elem_count; // the number of elements
    size_t elem_len;
    size_t read_ptr;
    size_t write_ptr;
};

stream *stream_create(size_t elem_len, size_t elem_count);

void *stream_read(stream *strm);

bool stream_write(stream *strm, void *data);

// populate the stream with nelem items
bool stream_populate(stream *strm, void *data, size_t nelem);

void *stream_peek(stream *strm, size_t off);

bool stream_resize(stream *strm, size_t new_elem_count);

void stream_reset(stream *strm);

bool stream_has_more(stream *strm);

void stream_destroy(stream *strm);

// Get a pointer to the current read position in the stream
void *stream_at(stream *strm);

#endif