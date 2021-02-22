#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>

namespace sofs20
{
    void grpRenameDirentry(int pih, const char *name, const char *newName)
    {
        soProbe(204, "%s(%d, %s, %s)\n", __FUNCTION__, pih, name, newName);

        /* replace the following line with your code */
        // binRenameDirentry(pih, name, newName);

        SOInode* pin = soGetInodePointer(pih);
        unsigned block_index = InodeNullReference;
        unsigned entry_index = InodeNullReference;

        SODirentry entries[DPB];

        // printf("name: %s", name);
        // printf("\nnewName: %s", newName);
        

        for (uint32_t i = 0; i < pin->size/sizeof(SODirentry); i++) {

            // printf("\nentries[i].name: %s", entries[i].name);
            
            if (i % DPB == 0)
            {
                soReadFileBlock(pih, i, entries);
            }

            if (strcmp(newName, entries[i].name) == 0) {
                    throw SOException(EEXIST, __FUNCTION__);
            }
            
            if (strcmp(name, entries[i].name) == 0) {
                block_index = i;
                entry_index = i;
            }
        }

        if (block_index == InodeNullReference) {
            throw SOException(ENOENT,__FUNCTION__);
        }

        strcpy(entries[entry_index].name, newName);
        soWriteFileBlock(pih, block_index, entries);
    }
};

