#ifndef __DARIC_H
#define __DARIC_H

#include "tube-client.h"
#include "Daric.h"
#include "DaricMem.h"

static void copy_daric()
{
    copro_memcpy((void *)DARIC_START, Daric, sizeof(Daric));
}

#endif
