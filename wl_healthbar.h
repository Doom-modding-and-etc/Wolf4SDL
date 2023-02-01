#include "version.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_HEALTHMETTER)
int HealthMeter_NewBar(void);

void HealthMeter_DeleteBar(int barId);

void HealthMeter_Reset(void);

void HealthMeter_SetBarFlashing(int barId, boolean flashing);

void HealthMeter_Draw(void);

int HealthMeter_NewBarEnemy(objtype* ob, int shapenum, int cropx,
    int cropy, int cropw, int croph);

int32_t HealthMeter_SaveTheGame(FILE* file, int32_t checksum);

int32_t HealthMeter_LoadTheGame(FILE* file, int32_t checksum);
#endif