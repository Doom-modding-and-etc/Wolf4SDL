#ifndef VEC_H_
#define VEC_H_

#include <assert.h>
#include <stdlib.h>

/* Summary
 *
 *  typedef vec_t(Type) Vec;
 *  Vec v = {0};
 *  unsigned vec_size (Vec);
 *  Type vec_at (Vec, unsigned);
 *  void vec_remove_unordered (Vec, unsigned);
 *  Type *vec_inc_top (Vec, Type);   Return pointer to new top
 *  void vec_push (Vec, Type, Obj);
 *  Type vec_pop_back (Vec);
 *  void vec_resize (Vec, unsigned);
 *  void vec_reserve (Vec, Type, unsigned);
 *  void vec_clear (Vec);
 *  void vec_free (Vec, Type);
 *
 *  `vec_at` evaluates to an lvalue.
 *  Data pointer access is &vec_at(v,0);
 */

/* inspired and much of it taken from kvec.h by attractive chaos */
#define vec_t(type) struct { unsigned alloc, sz; type *data; }
#define vec_size(v) ((const unsigned)(v).sz)
#define vec_at(v,i) ((v).data[vecI_bounds_check((v).sz, (i))])
#define vec_at_unchecked(v,i) ((v).data[i])
#define vec_remove_unordered(v,i) \
    do { \
        unsigned _vecI_i = vecI_bounds_check((v).sz, (i)); \
        (v).data[_vecI_i] = (v).data[--(v).sz]; \
    } while (0)

static inline void *
vecI_grow (unsigned membsz, unsigned *alloc, void *buf)
{
    /* If *alloc is 0, set to 8, else grow by a factor of 2 */
    *alloc = ((~!*alloc + 1) & 8) + (*alloc << 1);
    return realloc(buf, membsz * (*alloc));
}

static inline void *
vecI_reserve (unsigned membsz, unsigned rq, unsigned *alloc, void *buf)
{
    /* free if 0, else round to floor_log2(rq-1) and pass it on */
    if (rq == 0)
    {
        *alloc = 0, free(buf);
        return NULL;
    }
    rq--; while (rq & (rq - 1)) rq = rq & (rq - 1); *alloc = rq;
    return vecI_grow(membsz, alloc, buf);
}

#define vecI_inc_siz(v, t) \
    ((v).sz++, \
     ((v).sz > (v).alloc ? \
        (v).data = (t *)vecI_grow(sizeof(*(v).data), &(v).alloc, (v).data) \
      : 0))

#define vec_inc_top(v, t) (vecI_inc_siz(v, t), &(v).data[(v).sz-1])
#define vec_top(v) ((v).data[(v).sz-1])
#define vec_push(v,t,obj) (*vec_inc_top(v, t) = (obj))
#define vec_pop_back(v) (assert((v).sz > 0), (v).data[--(v).sz])
#define vec_reserve(v,t,rq) \
    ((v).data = (t *)vecI_reserve(sizeof(*(v).data), (rq), &(v).alloc, (v).data))
#define vec_resize(v,t,siz) (vec_reserve((v),t,(siz)), (v).sz = (siz))
#define vec_clear(v) ((v).sz = 0)
#define vec_free(v,t) vec_resize((v),t,0)

static inline unsigned
vecI_bounds_check (unsigned sz, unsigned idx)
{
    assert(idx < sz); (void)sz;
    return idx;
}

#endif /* VEC_H_ */
