#include "fileblocks.h"

#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpWriteFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(332, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);

        uint32_t block = soGetFileBlock(ih, fbn);

        if (block == BlockNullReference){
            block = soAllocFileBlock(ih, fbn);
        }
            
        soWriteDataBlock(block, buf);

        /* replace the following line with your code */
        //binWriteFileBlock(ih, fbn, buf);
    }
};

