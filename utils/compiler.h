#ifndef _COMPILER_
#define _COMPILER_

#ifndef VEC_RESIZE_FACTOR
#define VEC_RESIZE_FACTOR 2
#endif

#ifndef UMAP_BUCKET_LEN
#define UMAP_BUCKET_LEN 10
#endif

#ifndef USET_BUCKET_LEN
#define USET_BUCKET_LEN 10
#endif

#ifndef COMPILATION_STACK_SIZE
#define COMPILATION_STACK_SIZE 5
#endif

#ifndef ASSUME_FILES_INCLUDED
#define ASSUME_FILES_INCLUDED 5
#endif

#define ASSUME_CHILDREN_FILES_INCLUDED ASSUME_FILES_INCLUDED

// When mapping file contexts to file paths, having more buckets will be a better choice
// But if most buckets aren't used, it will also be a problem.
// We cannot make it dynamic yet so we will have to decide on some fixed size
#ifndef ASSUME_FILE_CONTEXT_MAP_BUCKET_COUNT
#define ASSUME_FILE_CONTEXT_MAP_BUCKET_COUNT 12
#endif

#endif