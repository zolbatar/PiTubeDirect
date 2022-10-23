#ifndef __DORTHBLOB_H
#define __DORTHBLOB_H

#include "tube-client.h"
#include "Dorth.h"
#include "DorthMem.h"

static void copy_dorth()
{
    copro_memcpy((void *)DORTH_START, Dorth, sizeof(Dorth));
}

#endif
