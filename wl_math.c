// WL_MATH.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_MATH)

#define MATRIXSTACK_MAX 10

typedef struct Math_s
{
    mat3f_t matCur;
    int matStackTop;
    mat3f_t matStack[MATRIXSTACK_MAX];
} Math_t;

static Math_t Math =
{
    { 
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
    }, // matCur
    0, // matStackTop
};

void Math_PushMatrix(void)
{
    if (Math.matStackTop >= MATRIXSTACK_MAX)
    {
        Quit("Cannot push into exhausted matrix stack!");
    }
    Math.matStack[Math.matStackTop++] = Math.matCur;
}

void Math_PopMatrix(void)
{
    if (Math.matStackTop <= 0)
    {
        Quit("Cannot pop from empty matrix stack!");
    }
    Math.matCur = Math.matStack[--Math.matStackTop];
}

void Math_PushMatrixIdent(void)
{
    Math_PushMatrix();
    Math.matCur = mat3f_ident();
}

void Math_TranslateMatrix(float tx, float ty)
{
    mat3f_t tm = mat3f_ident();
    
    CM(tm, 0, 2) = tx;
    CM(tm, 1, 2) = ty;

    Math.matCur = mat3f_mult(Math.matCur, tm);
}

void Math_ScaleMatrix(float sx, float sy)
{
    mat3f_t ts = mat3f_ident();
    
    CM(ts, 0, 0) = sx;
    CM(ts, 1, 1) = sy;

    Math.matCur = mat3f_mult(Math.matCur, ts);
}

void Math_ShearMatrix(float sx, float sy)
{
    mat3f_t ts = mat3f_ident();
    
    CM(ts, 0, 1) = sx;
    CM(ts, 1, 0) = sy;

    Math.matCur = mat3f_mult(Math.matCur, ts);
}

vec3f_t Math_MatrixMultPoint(vec3f_t pt)
{
    return mat3f_mult_point(Math.matCur, pt);
}

vec3f_t vec3f(float x, float y, float z)
{
    vec3f_t a;
    C(a, 0) = x;
    C(a, 1) = y;
    C(a, 2) = z;
    return a;
}

mat3f_t mat3f_ident(void)
{
    mat3f_t m = 
    {
        { 
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
        },
    };
    return m;
}

mat3f_t mat3f_mult(mat3f_t a, mat3f_t b)
{
    int i, j, k;
    mat3f_t c;

    memset(&c, 0, sizeof(c));
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 3; k++)
            {
                CM(c, i, j) += CM(a, i, k) * CM(b, k, j);
            }
        }
    }

    return c;
}

vec3f_t mat3f_mult_point(mat3f_t a, vec3f_t b)
{
    return vec3f(
        CM(a, 0, 0) * C(b, 0) + CM(a, 0, 1) * C(b, 1) + CM(a, 0, 2) * C(b, 2),
        CM(a, 1, 0) * C(b, 0) + CM(a, 1, 1) * C(b, 1) + CM(a, 1, 2) * C(b, 2),
        CM(a, 2, 0) * C(b, 0) + CM(a, 2, 1) * C(b, 1) + CM(a, 2, 2) * C(b, 2)
        );
}

vec3fixed_t vec3fixed_anglevec(int angle)
{
    fixed cosv, sinv;

    while (angle < 0)
    {
        angle += ANGLES;
    }
    while (angle >= ANGLES)
    {
        angle -= ANGLES;
    }

    cosv = costable[angle];
    sinv = sintable[angle];

    return vec3fixed(cosv, sinv, FP(0));
}

mat3fixed_t mat3fixed_zero(void)
{
    mat3fixed_t m = 
    {
        { 
            FP(0), FP(0), FP(0),
            FP(0), FP(0), FP(0),
            FP(0), FP(0), FP(0),
        },
    };
    return m;
}

mat3fixed_t mat3fixed_ident(void)
{
    mat3fixed_t m = 
    {
        { 
            FP(1), FP(0), FP(0),
            FP(0), FP(1), FP(0),
            FP(0), FP(0), FP(1),
        },
    };
    return m;
}

mat3fixed_t mat3fixed_diag(fixed a, fixed b, fixed c)
{
    mat3fixed_t m = mat3fixed_zero();
    CM(m, 0, 0) = a;
    CM(m, 1, 1) = b;
    CM(m, 2, 2) = c;
    return m;
}

vec3fixed_t mat3fixed_mult_point(mat3fixed_t a, vec3fixed_t b)
{
    return vec3fixed(
        FixedMul(CM(a, 0, 0), C(b, 0)) + 
        FixedMul(CM(a, 0, 1), C(b, 1)) +
        FixedMul(CM(a, 0, 2), C(b, 2)),
        FixedMul(CM(a, 1, 0), C(b, 0)) +
        FixedMul(CM(a, 1, 1), C(b, 1)) +
        FixedMul(CM(a, 1, 2), C(b, 2)),
        FixedMul(CM(a, 2, 0), C(b, 0)) +
        FixedMul(CM(a, 2, 1), C(b, 1)) +
        FixedMul(CM(a, 2, 2), C(b, 2))
        );
}

mat3fixed_t mat3fixed_rot(int angle)
{
    mat3fixed_t rotmat;
    fixed cosv, sinv;

    while (angle < 0)
    {
        angle += ANGLES;
    }
    while (angle >= ANGLES)
    {
        angle -= ANGLES;
    }

    cosv = costable[angle];
    sinv = sintable[angle];

    rotmat = mat3fixed_ident();
    C(rotmat, 0) = cosv;
    C(rotmat, 1) = -sinv;
    C(rotmat, 3) = sinv;
    C(rotmat, 4) = cosv;

    return rotmat;
}

mat3fixed_t mat3fixed_transpose(mat3fixed_t a)
{
    int i, j;
    mat3fixed_t b;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            CM(b, i, j) = CM(a, j, i);
        }
    }

    return b;
}

int Math_Log2(int x)
{
    int res = 0;
    while (x > 1)
    {
        res++;
        x >>= 1;
    }
    return res;
}

uint32_t Math_FixedLerpRgba(uint32_t a, uint32_t b, unsigned char blend)
{
    int i;
    uint32_t c = RGBA(0,0,0,255);
    unsigned char *pa, *pb, *pc;

    pa = (unsigned char *)&a;
    pb = (unsigned char *)&b;
    pc = (unsigned char *)&c;

    for (i = 0; i < 3; i++)
    {
        pc[i] = VBufBlendTable[blend][pa[i]][pb[i]];
    }

    return c;
}
#endif