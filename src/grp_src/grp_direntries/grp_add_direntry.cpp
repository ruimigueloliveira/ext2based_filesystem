#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs20
{
    void grpAddDirentry(int pih, const char *name, uint16_t cin)
    {
        soProbe(202, "%s(%d, %s, %u)\n", __FUNCTION__, pih, name, cin);

        SOInode* inodep = soGetInodePointer(pih);
        unsigned block_index = BlockNullReference;
        unsigned direntry_index = BlockNullReference;

        for (unsigned i = 0; i < inodep->size/BlockSize; i++){
            SODirentry direntry[DPB];
            soReadFileBlock(pih, i, direntry);
            
            for (unsigned j = 0; j < DPB; j++){
                if (block_index == BlockNullReference && direntry[j].name[0] == '\0'){
                    block_index = i;
                    direntry_index = j;
                }
            }
        }

        SODirentry direntry[DPB];

        if (block_index == BlockNullReference){
            direntry_index = 0;
            block_index = inodep->size/BlockSize;
            soAllocFileBlock(pih, block_index);
            inodep->size += BlockSize;

            for (unsigned i = 0; i < DPB; i++){
                strcpy(direntry[i].name, "");
                direntry[i].in = InodeNullReference;
            }
        }
        else{
            soReadFileBlock(pih, block_index, direntry);
        } 

        strcpy(direntry[direntry_index].name, name);
        direntry[direntry_index].in = cin;

        soWriteFileBlock(pih, block_index, direntry);

        /* replace the following line with your code */
        //binAddDirentry(pih, name, cin);
    }
};

