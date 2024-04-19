#include <stdlib.h>
#include <memory.h>

#define SCOPE_DEF(sz)\
typedef struct {\
    void* ptrs[sz];\
    unsigned int ptrs_size;\
} Scope_##sz;

SCOPE_DEF(100) //is 100 ptr slot enough?
SCOPE_DEF(1) //just one ptr slot is allowed

#define Scope Scope_100


#define X_malloc(type,a,size) a = (type*)malloc(sizeof(type) * size);

#define X_free_in(sc,a) \
sc.ptrs[sc.ptrs_size] = a;\
sc.ptrs_size++;

#define X_malloc_free_in(sc,type,a,size) X_malloc(type,a,size) X_free_in(sc,a)

#define X_free(sc) \
for (unsigned int i = 0; i < sc.ptrs_size; i++) {\
    free(sc.ptrs[i]);\
}\
sc.ptrs_size = 0;


//more ..
#define X_SCOPE_BEGIN { Scope sc = { 0 };
#define X_SCOPE_BEGIN_FOR(sz) { Scope_##sz sc = { 0 };
#define X_SCOPE_END X_free(sc);}
#define X_malloc_local(type,a,size) X_malloc_free_in(sc,type,a,size)
#define X_free_local(a) X_free_in(sc,a)
#define X_SCOPE_LAST(i) sc.ptrs[sc.ptrs_size-i-1]

float* GetAllocatedData(int sz) {
    return (float*)malloc(sizeof(float) * sz);
}

int main() {
#if 0
    Scope sc;
    {
        Scope* up = &sc;
        Scope sc = { 0 };

        int* a = (int*)malloc(sizeof(int) * 10);
        sc.ptrs[sc.ptrs_size] = a;
        sc.ptrs_size++;

        float* b = (float*)malloc(sizeof(float) * 10);
        sc.ptrs[sc.ptrs_size] = b;
        sc.ptrs_size++;

        float* c = (float*)malloc(sizeof(float) * 10);
        up->ptrs[up->ptrs_size] = &c;
        up->ptrs_size++;

        float* d = GetAllocatedData(5);
        sc.ptrs[sc.ptrs_size] = d;
        sc.ptrs_size++;


        for (unsigned int i = 0; i < sc.ptrs_size; i++) {
            free(sc.ptrs[i]);
        }
        sc.ptrs_size = 0;
    }
#endif

#if 1
    {
        Scope sc = { 0 };

        int* a;
        X_malloc_free_in(sc, int, a, 10);

        X_malloc_free_in(sc, int, a, 10);

        float* b;
        X_malloc_free_in(sc, float, b, 10);

        {
        Scope in = { 0 };

        X_malloc_free_in(in, float, b, 10);

        X_free(in)
        }

        float* d = GetAllocatedData(5);
        X_free_in(sc, d);

        X_free(sc);
    }
#endif

#if 1
    X_SCOPE_BEGIN

        int* a;
        X_malloc_local(int, a, 10);

        X_malloc_local(int, a, 10);

        float* b;
        X_malloc_local(float, b, 10);

        X_SCOPE_BEGIN_FOR(1)
            X_malloc_local(float, b, 10);
        X_SCOPE_END

        b = X_SCOPE_LAST(0);

        float* d = GetAllocatedData(5);
        X_free_local(d);

    X_SCOPE_END

#endif
}

