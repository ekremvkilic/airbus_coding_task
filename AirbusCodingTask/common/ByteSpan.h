#ifndef AIRBUS_CODING_TASK_COMMON_BYTE_SPAN_H
#define AIRBUS_CODING_TASK_COMMON_BYTE_SPAN_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t* data;
    size_t size;
} ByteSpan;

uint8_t SpanRead(const ByteSpan span, const size_t index)
{
    assert((index < span.size) && "Span Indexing Error!");
    return span.data[index];
}

void SpanWrite(ByteSpan span, const size_t index, const uint8_t byte_value)
{
    assert((index < span.size) && "Span Indexing Error!");
    span.data[index] = byte_value;
}

#endif // AIRBUS_CODING_TASK_COMMON_BYTE_SPAN_H
