// WL_BENCHHOBO.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_BENCHHOBO)
#include "wl_shade.h"

static statetype s_benchhobo[] =
{
    {false,SPR_HOBO_BENCH,3,(statefunc)SleepHobo_Think,NULL,&s_benchhobo[0]},
    {false,SPR_BENCH_BEGGER_WAKE1,10,NULL,NULL,&s_benchhobo[2]},
    {false,SPR_BENCH_BEGGER_WAKE2,10,NULL,(statefunc)SleepHobo_Awake,&s_benchhobo[3]},
    {false,SPR_STAT_45,10,NULL,NULL,&s_benchhobo[3]},
};

statetype *BenchHobo_States(void)
{
    return s_benchhobo;
}
#endif