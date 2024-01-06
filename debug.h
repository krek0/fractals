#include <stdlib.h>

#ifdef DEBUG
    #ifndef DEBUG_H
        #define DEBUG_H
        #define FREE myfree
        #define MALLOC mymalloc
        void* mymalloc(size_t size);
        void myfree(void *ptr);
    #endif
#else
    #define FREE free
    #define MALLOC malloc
#endif
