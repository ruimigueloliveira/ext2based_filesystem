#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs20{
    bool grpCheckDirEmpty(int ih){
        soProbe(205, "%s(%d)\n", __FUNCTION__, ih);

        /* replace the following line with your code */
        //return binCheckDirEmpty(ih);
    
        SOInode* in = soGetInodePointer(ih);
	
        for (unsigned i=0; i< in->size / BlockSize; i++) {
            SODirentry entries[DPB];
            soReadFileBlock(ih, i, entries);
            for (unsigned j=2; j < DPB; j++) {
                if (strcmp(entries[j].name, "\0") != 0) {
                    return false;
                }
            }
        }

    	return true;
    }
};

