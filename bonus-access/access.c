/* Порівняння швидкості доступу до пам'яті.
 * Запуск: ./access <seq|stride|rand> <розмір у MiB> */
#define _GNU_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

static double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static long major_faults(void)
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_majflt;
}

static uint64_t xorshift(uint64_t *state)
{
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return *state = x;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <seq|stride|rand> <MiB>\n", argv[0]);
        return 2;
    }
    const char *mode = argv[1];
    if (strcmp(mode, "seq") != 0 && strcmp(mode, "stride") != 0 && strcmp(mode, "rand") != 0) {
        fprintf(stderr, "unknown mode %s\n", mode);
        return 2;
    }
    char *end;
    errno = 0;
    long mib = strtol(argv[2], &end, 10);
    if (errno != 0 || end == argv[2] || *end != '\0' || mib <= 0 ||
        (uintmax_t)mib > SIZE_MAX / (1024 * 1024)) {
        fprintf(stderr, "invalid size: %s (expected a positive integer in MiB)\n", argv[2]);
        return 2;
    }
    size_t bytes = (size_t)mib * 1024 * 1024;
    size_t count = bytes / sizeof(uint32_t);
    size_t accesses = 64 * 1024 * 1024;

    uint32_t *array = malloc(bytes);
    if (array == NULL) {
        perror("malloc");
        return 1;
    }
    memset(array, 1, bytes);

    uint64_t rng = 88172645463325252ULL;
    uint64_t sum = 0;
    size_t stride = 4096 / sizeof(uint32_t);
    long faults_before = major_faults();
    double start = now_sec();

    if (strcmp(mode, "seq") == 0) {
        for (size_t i = 0; i < accesses; i++)
            sum += array[i % count];
    } else if (strcmp(mode, "stride") == 0) {
        for (size_t i = 0, idx = 0; i < accesses; i++, idx += stride)
            sum += array[idx % count];
    } else {
        for (size_t i = 0; i < accesses; i++)
            sum += array[xorshift(&rng) % count];
    }

    double elapsed = now_sec() - start;
    long faults = major_faults() - faults_before;
    printf("%-6s %6zu MiB  %.3f s  %.2f ns/access  major faults: %ld  (sum %llu)\n", mode,
           bytes >> 20, elapsed, elapsed * 1e9 / accesses, faults, (unsigned long long)sum);
    free(array);
    return 0;
}
