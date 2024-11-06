#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "umap.h"

size_t string_hash(void *key, size_t bucket_count) {
    char *str = (char *)key;
    size_t hash = 0;
    while (*str) {
        hash = (hash * 31 + *str++) % bucket_count;
    }
    return hash;
}

bool string_cmp(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

void string_cleanup(void *ptr) {
    free(ptr);
}

char *strdup_safe(const char *str) {
    char *copy = strdup(str);
    if (!copy) {
        report_internal_error("Failed to allocate memory for string.");
        exit(EXIT_FAILURE);
    }
    return copy;
}

void run_tests() {
    umap *map = umap_create(10, string_hash, string_cmp, string_cleanup, string_cleanup);
    if (!map) {
        printf("Failed to initialize map.\n");
        return;
    }

    printf("Running tests...\n");

    // Test insertion of single element
    char *key1 = strdup_safe("key1");
    char *val1 = strdup_safe("value1");
    if (!umap_insert(map, key1, val1)) {
        printf("Failed to insert initial element.\n");
    }

    // Test retrieval of single element
    char *retrieved = (char *)umap_find(map, "key1");
    if (!retrieved || strcmp(retrieved, "value1") != 0) {
        printf("Test failed: Retrieved value does not match.\n");
    } else {
        printf("Test passed: Single insert/retrieve.\n");
    }

    // Test updating existing key
    char *val1_updated = strdup_safe("value1_updated");
    if (!umap_insert(map, key1, val1_updated)) {
        printf("Failed to update existing key.\n");
    }
    retrieved = (char *)umap_find(map, "key1");
    if (!retrieved || strcmp(retrieved, "value1_updated") != 0) {
        printf("Test failed: Updated value does not match.\n");
    } else {
        printf("Test passed: Update existing key.\n");
    }

    // Test insertion and retrieval of multiple keys
    char *key2 = strdup_safe("key2");
    char *val2 = strdup_safe("value2");
    umap_insert(map, key2, val2);

    char *key3 = strdup_safe("key3");
    char *val3 = strdup_safe("value3");
    umap_insert(map, key3, val3);

    retrieved = (char *)umap_find(map, "key2");
    if (!retrieved || strcmp(retrieved, "value2") != 0) {
        printf("Test failed: Retrieved value for key2 does not match.\n");
    } else {
        printf("Test passed: Insert multiple keys.\n");
    }

    retrieved = (char *)umap_find(map, "key3");
    if (!retrieved || strcmp(retrieved, "value3") != 0) {
        printf("Test failed: Retrieved value for key3 does not match.\n");
    }

    // Test edge cases
    if (umap_find(map, "nonexistent_key") != NULL) {
        printf("Test failed: Nonexistent key returned a value.\n");
    } else {
        printf("Test passed: Nonexistent key returns NULL.\n");
    }
    
    // Test cleanup function
    umap_destroy(map);
    printf("All tests completed.\n");
}

int main() {
    run_tests();
    return 0;
}
