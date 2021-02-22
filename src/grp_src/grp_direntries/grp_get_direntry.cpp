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
    uint16_t grpGetDirentry(int pih, const char *name)
    {
        soProbe(201, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        /* replace the following line with your code */
        //return binGetDirentry(pih, name);

        SOInode* pointer = soGetInodePointer(pih);
        SODirentry entries[DPB]; //array de entries
        uint16_t n = 0;

        if(pointer -> size%BlockSize != 0){ //se o resto for != 0, quer dizer que os blocos nao estao 100% preenchidos
            n = pointer->size/BlockSize + 1; //por isso o n toma o valor do proximo bloco (+1)
        }else{
            n = pointer -> size/BlockSize; //caso contrario, o n tem o valor normal
        }

        for(unsigned i=0; i < n; i++){
            soReadFileBlock(pih, i, entries);
            for(unsigned j=0; j<DPB; j++){
                if(strcmp(entries[j].name, name) == 0){ //se o nome introduzido corresponder a algum nome do array de direntries
                    return entries[j].in; //retorna o numero do child inode
                }
            }
        }

        
        
        return InodeNullReference; //se nao encontrar nenhum com o mesmo nome retorna null

    }
};

