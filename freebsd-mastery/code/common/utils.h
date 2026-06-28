#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* Affiche l'erreur et quitte si la condition est vraie. */
#define DIE_IF(cond, msg)                                   \
    do {                                                    \
        if (cond) {                                         \
            fprintf(stderr, "%s:%d: %s: %s\n",             \
                    __FILE__, __LINE__, (msg), strerror(errno)); \
            exit(EXIT_FAILURE);                             \
        }                                                   \
    } while (0)

/* Affiche l'erreur et quitte si la valeur est < 0. */
#define CHECK(val, msg) DIE_IF((val) < 0, (msg))

/* Affiche l'erreur et quitte si le pointeur est NULL. */
#define CHECK_NULL(ptr, msg) DIE_IF((ptr) == NULL, (msg))

/* Alloue ou quitte. */
static inline void *xmalloc(size_t n) {
    void *p = malloc(n);
    CHECK_NULL(p, "malloc");
    return p;
}

static inline void *xcalloc(size_t nmemb, size_t size) {
    void *p = calloc(nmemb, size);
    CHECK_NULL(p, "calloc");
    return p;
}

/* Lit exactement n octets (boucle sur EINTR/short read). */
ssize_t read_all(int fd, void *buf, size_t n);

/* Écrit exactement n octets (boucle sur EINTR/short write). */
ssize_t write_all(int fd, const void *buf, size_t n);

#endif /* UTILS_H */
