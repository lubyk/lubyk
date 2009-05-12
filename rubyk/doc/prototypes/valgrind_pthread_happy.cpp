#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void* start(void* arg) {
        printf("started with:'%s'\n", (const char*)arg);
        return (void*) 0;
}

int main() {
        pthread_t thread;
        int err;
        const size_t SIZE=15;
        char* msg = (char*)calloc(SIZE, sizeof(char));
        strncpy(msg, "heap thread", SIZE);
        if ((err = pthread_create(&thread, NULL, start, msg))) {
                puts("thread creation failed");
        }
        pthread_join(thread, NULL);
        free(msg);
}

