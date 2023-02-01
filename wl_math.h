#include "version.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_MATH)

#define X(a) (a).v[0]
#define Y(a) (a).v[1]
#define Z(a) (a).v[2]
#define C(a, i) (a).v[i]

#define Sq(a) ((a) * (a))

/* access numbered component of vector */
#define C(a, i) (a).v[i]

/* access (row,col) component of matrix */
#define CM(a, r, c) (a).v[(r) * 3 + (c)]

typedef struct mat3f_s
{
    float v[3 * 3];
} mat3f_t;

typedef struct vec3f_s
{
    float v[3];
} vec3f_t;

typedef struct vec3fixed_s
{
    fixed v[3];
} vec3fixed_t;

typedef struct mat3fixed_s
{
    fixed v[3 * 3];
} mat3fixed_t;

void Math_PushMatrix(void);

void Math_PopMatrix(void);

void Math_PushMatrixIdent(void);

void Math_TranslateMatrix(float tx, float ty);

void Math_ScaleMatrix(float sx, float sy);

void Math_ShearMatrix(float sx, float sy);

vec3f_t Math_MatrixMultPoint(vec3f_t pt);

vec3f_t Math_MatrixMultPoint(vec3f_t pt);

vec3f_t vec3f(float x, float y, float z);

mat3f_t mat3f_ident(void);

mat3f_t mat3f_mult(mat3f_t a, mat3f_t b);

vec3f_t mat3f_mult_point(mat3f_t a, vec3f_t b);

static inline vec3fixed_t vec3fixed(fixed x, fixed y, fixed z)
{
    vec3fixed_t a;
    C(a, 0) = x;
    C(a, 1) = y;
    C(a, 2) = z;
    return a;
}

static inline vec3fixed_t vec3fixed_zero(void)
{
    vec3fixed_t a = { 0, 0, 0 };
    return a;
}

static inline vec3fixed_t vec3fixed_add(vec3fixed_t a, vec3fixed_t b)
{
    return vec3fixed(
        C(a, 0) + C(b, 0),
        C(a, 1) + C(b, 1),
        C(a, 2) + C(b, 2)
    );
}

static inline vec3fixed_t vec3fixed_subtract(vec3fixed_t a, vec3fixed_t b)
{
    return vec3fixed(
        C(a, 0) - C(b, 0),
        C(a, 1) - C(b, 1),
        C(a, 2) - C(b, 2)
    );
}

static inline vec3fixed_t vec3fixed_negate(vec3fixed_t a)
{
    return vec3fixed(-C(a, 0), -C(a, 1), -C(a, 2));
}

static inline fixed vec3fixed_dot(vec3fixed_t a, vec3fixed_t b)
{
    return fixedpt_mul(C(a, 0), C(b, 0)) + fixedpt_mul(C(a, 1), C(b, 1)) +
        fixedpt_mul(C(a, 2), C(b, 2));
}

static inline vec3fixed_t vec3fixed_scale(vec3fixed_t a, fixed b)
{
    return vec3fixed(fixedpt_mul(X(a), b), fixedpt_mul(Y(a), b), fixedpt_mul(Z(a), b));
}

static inline vec3fixed_t vec3fixed_div(vec3fixed_t a, fixed b)
{
    return vec3fixed(fixedpt_div(X(a), b), fixedpt_div(Y(a), b), fixedpt_div(Z(a), b));
}

vec3fixed_t vec3fixed_anglevec(int32_t angle);

static inline fixed vec3fixed_length(vec3fixed_t a)
{
    return fixedpt_sqrt(vec3fixed_dot(a, a));
}

static inline vec3fixed_t vec3fixed_normalize(vec3fixed_t a)
{
    fixed n;

    n = fixedpt_sqrt(vec3fixed_dot(a, a));
    return n ? vec3fixed(fixedpt_div(X(a), n),
        fixedpt_div(Y(a), n), fixedpt_div(Z(a), n)) :
        a;
}

static inline vec3fixed_t vec3fixed_unit(vec3fixed_t a, vec3fixed_t b)
{
    return vec3fixed_normalize(vec3fixed_subtract(b, a));
}

mat3fixed_t mat3fixed_zero(void);

mat3fixed_t mat3fixed_ident(void);

mat3fixed_t mat3fixed_diag(fixed a, fixed b, fixed c);

vec3fixed_t mat3fixed_mult_point(mat3fixed_t a, vec3fixed_t b);

static inline vec3fixed_t mat3fixed_mult_point2d(mat3fixed_t a, vec3fixed_t b)
{
    return vec3fixed(
        fixedpt_mul(CM(a, 0, 0), C(b, 0)) + fixedpt_mul(CM(a, 0, 1), C(b, 1)),
        fixedpt_mul(CM(a, 1, 0), C(b, 0)) + fixedpt_mul(CM(a, 1, 1), C(b, 1)),
        0);
}

mat3fixed_t mat3fixed_rot(fixed angle);

mat3fixed_t mat3fixed_transpose(mat3fixed_t a);

uint32_t Math_FixedLerpRgba(uint32_t a, uint32_t b, unsigned char blend);
#endif