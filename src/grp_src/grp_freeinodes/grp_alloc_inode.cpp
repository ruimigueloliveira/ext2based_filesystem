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
#include <sys/stat.h>
#include <string.h>

#include <iostream>

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20

{
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

    uint16_t grpAllocInode(uint32_t mode)
    {
        soProbe(401, "%s(0x%x)\n", __FUNCTION__, mode);

        /* replace the following line with your code */
        // return binAllocInode(mode);

        SOSuperblock* sbp = soGetSuperblockPointer();

        uint32_t type = mode & ~0777;

        if(!(type == S_IFREG || type == S_IFDIR || type == S_IFLNK)){
            printf("\nerro1");
            throw SOException(EINVAL, __FUNCTION__);
        }

        if(sbp->ifree == 0){
            printf("\nerro2");
            sbp->iidx=1;
            throw SOException(ENOSPC, __FUNCTION__);
        }

        uint16_t inode_id = sbp->iidx+1;
        int inode_handler = soOpenInode(inode_id);
        SOInode* inodep = soGetInodePointer(inode_handler);

        inodep->mode = mode;
        inodep->owner = getuid();
        inodep->group = getgid();
        inodep->atime = time(NULL);
        inodep->ctime = time(NULL);
        inodep->mtime = time(NULL);
        
        uint32_t bit = 1;
        uint32_t n = (sbp->iidx)%(sbp->itotal);
        uint32_t i;
        
        while (bit!=0)
        {
            i = n/32; // indice do bitmap
            // printf("\nindice do bitmap: %d", i);

            uint32_t posicao = n%32; // posição do bit (0-31)
            // printf("\nposição do bit (0-31): %d", posicao);

            // printf("\nconteudo binario do bitmap[%d]: ", i);
            // printBits(sizeof(sbp->ibitmap[i]), &sbp->ibitmap[i]);

            // printf("conteudo hexa do bitmap[%d]: %x", i, sbp->ibitmap[i]);
            // printf("\n(1<<posicao): %d", (1<<posicao));

            bit = (sbp->ibitmap[i]) & (1<<posicao);

            // printf("\nvalor do bit %d: %d", posicao, bit);

            if (bit == 0)
            {
                (sbp->ibitmap[i]) = (sbp->ibitmap[i]) | (1<<posicao);
                // printf("\nconteudo binario do bitmap[%d]: ", i);
                // printBits(sizeof(sbp->ibitmap[i]), &sbp->ibitmap[i]);
                sbp->ifree = sbp->ifree - 1;
                (sbp->iidx) = ((32*i)+posicao)%(sbp->itotal);
                // printf("\niidx: %d", (sbp->iidx));
            }
            
            n = (n+1)%(sbp->itotal);

            // printf("n: %d\n", n);
        }
        
        soSaveInode(inode_handler);
        soSaveSuperblock();

        // printf("\nconteudo binario do bitmap[%d]: ", i);
        // printBits(sizeof(sbp->ibitmap[i]), &sbp->ibitmap[i]);
        
        return inode_id;
    }
};

