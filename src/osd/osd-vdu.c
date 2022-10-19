#include "osd-vdu.h"
#include "osd-tube.h"
#include "../framebuffer/screen_modes.h"

void VDUClg()
{
    TubeOSWRCH(22);
    TubeOSWRCH(0);
}
