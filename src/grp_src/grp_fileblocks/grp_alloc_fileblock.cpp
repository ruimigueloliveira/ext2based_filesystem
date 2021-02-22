#include "fileblocks.h"

#include "freedatablocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <errno.h>

#include <iostream>

namespace sofs20
{

    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);

    uint32_t i1Size = N_INDIRECT * RPB;                 //tamanho da array indireto 
    uint32_t i2Size = N_DOUBLE_INDIRECT * (RPB * RPB);  //tamanho do array duplamente indireto (ao quadrado)
    /* ********************************************************* */

    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* replace the following two lines with your code */
        //return binAllocFileBlock(ih, fbn);
        if(fbn>= N_DIRECT + i1Size + i2Size){               //caso não caiba num inode
            throw SOException(EINVAL, __FUNCTION__);
        }

        SOInode *ip = soGetInodePointer(ih);
        uint32_t ref_to_dataBlock;

        if(fbn< N_DIRECT){
            ref_to_dataBlock = soAllocDataBlock();          //para o caso de ser direto, pode alocar diretamente
            ip-> blkcnt += 1;                               //total number of blocks used ++
            ip->d[fbn] = ref_to_dataBlock;
        }
        else if(fbn <N_DIRECT + i1Size){
            ref_to_dataBlock = grpAllocIndirectFileBlock(ip, fbn);
        }else{
            ref_to_dataBlock = grpAllocDoubleIndirectFileBlock(ip, fbn);
        }
        soSaveInode(ih);
        return ref_to_dataBlock;
    }

    /* ********************************************************* */

    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        uint32_t block_reference;
        uint32_t i1_idx = (afbn - N_DIRECT) /RPB;
        uint32_t rdb_index = (afbn - N_DIRECT) - (RPB * i1_idx);
        uint32_t rdb_refs[RPB];         //pointer to the buffer where the data must be read into
        uint32_t ref_to_dataBlock;


        if(ip->i1[i1_idx] == BlockNullReference ){ // CONFIRMAR SE É BlockNullReference  | InodeNullReference 
            block_reference = soAllocDataBlock();
            ip-> blkcnt += 1;
            ip-> i1[i1_idx] = block_reference;
            for(unsigned long i =0; i< RPB; i++){
                rdb_refs[i] = BlockNullReference; // CONFIRMAR SE É BlockNullReference  | InodeNullReference 
            }
        }else{
            block_reference = ip->i1[i1_idx];
            soReadDataBlock(block_reference, rdb_refs);
        }
        
        ref_to_dataBlock= soAllocDataBlock();
        ip->blkcnt +=1;
        rdb_refs[rdb_index] = ref_to_dataBlock;
        soWriteDataBlock(block_reference, rdb_refs);
        return ref_to_dataBlock;
    }


    /* ********************************************************* */

    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        uint32_t i2_blck_ref, i1_blck_ref;
        uint32_t i2_index = (afbn - N_DIRECT - i1Size) / (RPB * RPB);
        uint32_t i1_index = ((afbn - N_DIRECT - i1Size) - (RPB * RPB * i2_index)) / RPB;
        uint32_t rdb_index = ((afbn - N_DIRECT - i1Size) - (RPB * RPB * i2_index)) - (RPB * i1_index);
        uint32_t dataBlock_ref;
        uint32_t i1_refs[RPB], rdb_refs[RPB];

        if (ip->i2[i2_index] == BlockNullReference) {
            i2_blck_ref = soAllocDataBlock();
            ip->blkcnt += 1;
            ip->i2[i2_index] = i2_blck_ref;
            for (unsigned long i = 0; i < RPB; i++) {
                i1_refs[i] = BlockNullReference;
            }
        }
        else {
            i2_blck_ref = ip->i2[i2_index];
            soReadDataBlock(i2_blck_ref, i1_refs);
        }

        if (i1_refs[i1_index] == BlockNullReference) {
            i1_blck_ref = soAllocDataBlock();
            ip->blkcnt += 1;
            i1_refs[i1_index] = i1_blck_ref;
            for (unsigned long i = 0; i < RPB; i++) {
                rdb_refs[i] = BlockNullReference;
            }
        }
        else {
            i1_blck_ref = i1_refs[i1_index];
            soReadDataBlock(i1_blck_ref, rdb_refs);
        }

        dataBlock_ref = soAllocDataBlock();
        ip->blkcnt += 1;
        rdb_refs[rdb_index] = dataBlock_ref;

        soWriteDataBlock(i1_blck_ref, rdb_refs);
        soWriteDataBlock(i2_blck_ref, i1_refs);
        return dataBlock_ref;

        /* replace the following two lines with your code */
        //throw SOException(ENOSYS, __FUNCTION__); 
        //return 0;
    }
};

