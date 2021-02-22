/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freedatablocks.h"
#include "core.h"
#include "devtools.h"
#include "daal.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

namespace sofs20{
    /* only fill the current block to its end */
    void grpDepleteInsertionCache(void){
        soProbe(444, "%s()\n", __FUNCTION__);

        /* replace the following line with your code */
        //binDepleteInsertionCache();

        SOSuperblock* sbp = soGetSuperblockPointer();

        // Nothing is done if the cache is not full
        if(sbp->insertion_cache.idx == REF_CACHE_SIZE - 1 && sbp->insertion_cache.ref[REF_CACHE_SIZE - 1] != BlockNullReference){
            // Only a single block of the reference table is processed, even if it has no room to empty the insertion cache
            // The block processed is the last with references
            uint32_t refTableSize = sbp->rt_size;
            uint32_t refBlockIndex = sbp->reftable.blk_idx;
            uint32_t refCellIndex = sbp->reftable.ref_idx;
            uint32_t refCount = sbp->reftable.count; 

            uint32_t firstBlockRefCount = RPB - refCellIndex;

            if(firstBlockRefCount <= refCount){
                uint32_t nextCellIndex = refCellIndex + refCount;
                uint32_t* refBlockPointer = soGetReferenceBlockPointer(refBlockIndex);

                for(uint32_t cellIndex = nextCellIndex; cellIndex < RPB && sbp->insertion_cache.idx > 0; cellIndex++){ 
                    refBlockPointer[cellIndex] = sbp->insertion_cache.ref[sbp->insertion_cache.idx];
                    
                    // After transferring a reference from A to B, the value in A becomes BlockNullReference 
                    sbp->insertion_cache.ref[sbp->insertion_cache.idx] = BlockNullReference;
                    sbp->insertion_cache.idx--;
                    sbp->reftable.count++;
                }
                soSaveReferenceBlock();
            }
            else{
                uint32_t remRefs = refCount - firstBlockRefCount;
                uint32_t remRefBlocks = remRefs / RPB;
                uint32_t nextCellIndex = RPB;

                if(remRefs % RPB != 0){
                    remRefBlocks++;
                    nextCellIndex = (remRefs % RPB) + 1;
                }
                uint32_t lastRefBlockIndex = refBlockIndex + remRefBlocks;

                // Be aware of the circularity of the reference table                 
                if(lastRefBlockIndex >= refTableSize){
                    lastRefBlockIndex -= refTableSize;
                }

                uint32_t* refBlockPointer = soGetReferenceBlockPointer(lastRefBlockIndex);

                for(uint32_t cellIndex = nextCellIndex; cellIndex < RPB && sbp->insertion_cache.idx > 0; cellIndex++){
                    refBlockPointer[cellIndex] = sbp->insertion_cache.ref[sbp->insertion_cache.idx];

                    sbp->insertion_cache.ref[sbp->insertion_cache.idx] = BlockNullReference;
                    sbp->insertion_cache.idx--;
                    sbp->reftable.count++;
                }
                soSaveReferenceBlock();
            }
            soSaveSuperblock();
        }
    }
};