#include "grp_mksofs.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"
#include <cmath>
#include <inttypes.h>
#include <iostream>

namespace sofs20{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & dbtotal){
        soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);

        /* replace the following line with your code */
        //binComputeStructure(ntotal, itotal, dbtotal);
        
        // itotal must be lower than or equal to the rounded up value of ntotal/8
        // itotal must be greater than or equal to IPB
        // if, at entry, the proposed value for itotal is 0, value ntotal/16 should be used
        if(itotal > ceil((double) ntotal / 8) || itotal < IPB || itotal == 0){
            itotal = ntotal / 16;
        }
              
        // itotal must be rounded up to a multiple of 32
        if(itotal % 32 != 0){
            itotal = ceil((double) itotal/32)*32;
        }

        // itotal must be rounded up to a multiple of IPB
        if(itotal % IPB != 0){
            itotal = ceil((double) itotal/IPB)*IPB;
        }

        uint32_t iBlocks = itotal / IPB;
        uint32_t initRefs = 1 + iBlocks + 1 + REF_CACHE_SIZE;
        
        if(initRefs < ntotal){
            uint32_t lastBlocks = ntotal - 1 - iBlocks - 1 - REF_CACHE_SIZE;
            uint32_t nbref = lastBlocks / RPB;
            
            if(lastBlocks % RPB != 0){
                nbref += 1;
            }

            uint32_t dbFree = lastBlocks - nbref;
            dbtotal = 1 + REF_CACHE_SIZE + dbFree;

            uint32_t totalBlocks = dbtotal + 1 + iBlocks + nbref;
            
            // If, after making the partition, a spare block remains, it should be assigned to the inode table
            if(totalBlocks > ntotal){
                itotal = itotal + (ntotal-totalBlocks) * IPB;
            }
        }
        else{
            dbtotal = ntotal - (1 + iBlocks);
        }
    }
};