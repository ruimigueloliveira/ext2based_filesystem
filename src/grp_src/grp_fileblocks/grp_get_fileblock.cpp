#include "fileblocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"
#include <errno.h>

namespace sofs20{
    
    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t fbn);
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t fbn);

    // ih -> inode handler
    // fbn -> file block number
    // Get the data block number corresponding to the given file block
    uint32_t grpGetFileBlock(int ih, uint32_t fbn) {
        soProbe(301, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* replace the following line with your code */
        // return binGetFileBlock(ih, fbn);
        
        uint32_t i1size = N_INDIRECT * RPB;
        uint32_t i2size = N_DOUBLE_INDIRECT * RPB * RPB;

        // EINVAL -> if fbn is not valid
        if (fbn >= N_DIRECT + i1size + i2size) {
            throw SOException(EINVAL, __FUNCTION__);
        }

        SOInode* inode = soGetInodePointer(ih);
        uint32_t block_ref;

        if (fbn < N_DIRECT) {
            block_ref = inode->d[fbn];
        }
        else if (fbn < N_DIRECT+i1size) {
            block_ref = grpGetIndirectFileBlock(inode, fbn - N_DIRECT);
        }
        else {
            block_ref = grpGetDoubleIndirectFileBlock(inode, fbn - N_DIRECT - i1size);
        }
        return block_ref;
    }

    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t afbn){
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* replace the following two lines with your code */
        // throw SOException(ENOSYS, __FUNCTION__); 
        // return 0;

        uint32_t i1_idx = afbn / RPB;
        uint32_t ref_idx = afbn % RPB;

        if (ip->i1[i1_idx] == BlockNullReference) {
            return BlockNullReference;
        }

        uint32_t ref[RPB];
        soReadDataBlock(ip->i1[i1_idx], &ref);
        return ref[ref_idx];
    }

    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn){
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* replace the following two lines with your code */
        // throw SOException(ENOSYS, __FUNCTION__); 
        // return 0;

        uint32_t sqr_rpb = RPB * RPB;
        uint32_t i2_idx = afbn / sqr_rpb;
        uint32_t i1fdb_idx = (afbn / RPB) % RPB;
        uint32_t rdb_index = afbn % RPB % RPB;

        if (ip->i2[i2_idx] == BlockNullReference) {
            return BlockNullReference;
        }

        uint32_t ref_i1[RPB];
        soReadDataBlock(ip->i2[i2_idx], &ref_i1);
        if (ref_i1[i1fdb_idx] == BlockNullReference) {
            return BlockNullReference;
        }

        uint32_t ref[RPB];
        soReadDataBlock(ref_i1[i1fdb_idx], &ref);
        return ref[rdb_index];
    }
};