#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpFillSuperblock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t dbtotal)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, dbtotal);
    
        SOSuperblock sb;
        strncpy(sb.name, name, PARTITION_NAME_LEN);

        sb.magic = MAGIC_NUMBER;
        sb.version = VERSION_NUMBER;
        sb.mntstat = 0;
        sb.ntotal = ntotal;
        sb.itotal = itotal;

        sb.dbp_start = (itotal/IPB) + 1;
        sb.dbtotal = dbtotal;
        sb.dbfree= sb.dbtotal-1;

        sb.ifree  =  itotal - 1;
        sb.iidx = 0;

        sb.reftable.ref_idx=0;  //Index of first cell, within first block, with references
        sb.insertion_cache.idx=0;   //index of the first empty insertion cache position

        sb.rt_start = (itotal/IPB) + dbtotal + 1; // por causa do 0---> First block of the reference table
        sb.rt_size = ntotal - (itotal/IPB) - dbtotal - 1; // n de blocos da ref table
        
        //inicialização dos dados da retrieval cache
        if(sb.dbfree <= REF_CACHE_SIZE)
        {
            sb.reftable.blk_idx = 0;
            sb.reftable.count = 0;
            sb.retrieval_cache.idx = REF_CACHE_SIZE - sb.dbfree;
        }else{
            sb.reftable.blk_idx = 0;
            uint32_t db_restantes = (sb.dbfree - REF_CACHE_SIZE);
            sb.reftable.count = db_restantes;
            sb.retrieval_cache.idx = 0;
        }

        //Preenchimento da retrieval cache
        uint32_t first_data_block_index = 1;
        for(uint32_t i = 0; i < REF_CACHE_SIZE; i++)
        {
            if(i < sb.retrieval_cache.idx){
                sb.retrieval_cache.ref[i] = BlockNullReference;
            }else{
                sb.retrieval_cache.ref[i] = first_data_block_index++;
            }
        }

        //inode allocation bitmap
        for(int i=0; i< MAX_INODES/32; i++){
            if(i==0){
                sb.ibitmap[i]=1;
            }else{
                sb.ibitmap[i]=0;
            }
        }
        //preenchimento da insertition cache
        for(int i=0; i<REF_CACHE_SIZE; i++){
            sb.insertion_cache.ref[i] = BlockNullReference;
        }   

        soWriteRawBlock(0,&sb);
        /* replace the following line with your code */
       // binFillSuperblock(name, ntotal, itotal, dbtotal);
    }   
};
