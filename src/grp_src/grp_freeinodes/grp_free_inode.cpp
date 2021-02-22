/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freeinodes.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20
{
    void grpFreeInode(uint16_t in)
    {
        soProbe(402, "%s(%u)\n", __FUNCTION__, in);

        // TODO: Throw EINVAL exception if inode number is not valid
        /*if(...){
            throw SOException(EINVAL, __FUNCTION__);
        }*/
        // end TODO

        SOSuperblock* sbp = soGetSuperblockPointer();
        int pih = soOpenInode(in);
        SOInode* inodep = soGetInodePointer(pih);

        inodep->mode = 0;
        inodep->lnkcnt = 0;
        inodep->owner = 0;
        inodep->group = 0;
        inodep->size = 0;
        inodep->blkcnt = 0;
        inodep->atime = 0;
        inodep->mtime = 0;
        inodep->ctime = 0;

        for (int i = 0; i < N_DIRECT; i++){
            inodep->d[i] = BlockNullReference;
        }
        for (int i = 0; i < N_INDIRECT; i++){
            inodep->i1[i] = BlockNullReference;
        }
        for (int i = 0; i < N_DOUBLE_INDIRECT; i++){
            inodep->i2[i] = BlockNullReference;
        }

        // TOCHECK: The corresponding bit of the ibitmap field is put at zero
        uint32_t bit = 0;
        uint32_t n = (sbp->iidx)%(sbp->itotal);
        uint32_t i;
        
        while (bit!=1){
            i = n/32; // ibitmap index

            uint32_t posicao = n%32; // bit position (0-31)

            bit = (sbp->ibitmap[i]) & (0<<posicao);

            if (bit == 1){
                (sbp->ibitmap[i]) = (sbp->ibitmap[i]) | (0<<posicao);
                sbp->ifree = sbp->ifree + 1;
            }
            
            n = (n+1)%(sbp->itotal);
        }
        // end TOCHECK

        soSaveInode(pih);
        soSaveSuperblock();

        /* replace the following line with your code */
        // binFreeInode(in);
    }
};

