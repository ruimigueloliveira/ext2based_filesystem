#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    /*
       filling in the contents of the root directory:
       the first 2 entries are filled in with "." and ".." references
       the other entries are empty.
       If rdsize is 2, a second block exists and should be filled as well.
       */
    void grpFillRootDir(uint32_t itotal)
    {
        soProbe(606, "%s(%u)\n", __FUNCTION__, itotal);

        SODirentry direntry[DPB];

        strcpy(direntry[0].name, ".");
        direntry[0].in = 0;

        strcpy(direntry[1].name, "..");
        direntry[1].in = 0;

        for (unsigned long i = 2; i < DPB; i++){
            strcpy(direntry[i].name, "");
            direntry[i].in = 0x0;
        }

        soWriteRawBlock(itotal/IPB + 1, direntry);

        /* replace the following line with your code */
        // binFillRootDir(itotal);
    }
};

