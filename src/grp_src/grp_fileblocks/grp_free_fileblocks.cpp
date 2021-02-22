#include "fileblocks.h"

#include "freedatablocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>

namespace sofs20
{
//#if false
    /* free all blocks between positions ffbn and RPB - 1
     * existing in the block of references given by i1.
     * Return true if, after the operation, all references become BlockNullReference.
     * It assumes i1 is valid.
     */
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn);

    /* free all blocks between positions ffbn and RPB**2 - 1
     * existing in the block of indirect references given by i2.
     * Return true if, after the operation, all references become BlockNullReference.
     * It assumes i2 is valid.
     */
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn);
//#endif

    /* ********************************************************* */

    unsigned freeFB = 0; //numero total de fileblocks libertados

    void grpFreeFileBlocks(int ih, uint32_t ffbn)
    {
        soProbe(303, "%s(%d, %u)\n", __FUNCTION__, ih, ffbn);

        /* replace the following line with your code */
        //binFreeFileBlocks(ih, ffbn);

        SOInode* pointer = soGetInodePointer(ih);
        
        int dInicio=0;
        int i1Inicio=-1;
        int i2Inicio=-1; 
        

        if(ffbn < N_DIRECT){ //apagar a partir do d[]
            dInicio = ffbn;
        }else if(ffbn < N_DIRECT + N_INDIRECT*RPB){ //apagar a partir do i1[]
            dInicio = N_DIRECT;
            i1Inicio = (ffbn-N_DIRECT)/RPB;
        }else{ //apagar a partir do i2[]
            dInicio = N_DIRECT;
            i1Inicio = N_INDIRECT;
            i2Inicio = (ffbn-N_DIRECT-N_INDIRECT*RPB)/(RPB*RPB);
        }

        //d[]
        for(unsigned i=0; i<N_DIRECT; i++){
            if((int)i >= dInicio && pointer->d[i] != BlockNullReference){ //apaga apenas a partir do ffbn e verifica se está vazio
                soFreeDataBlock(pointer->d[i]); //se nao estiver vazio apaga
                freeFB++;
                pointer->d[i] = BlockNullReference; //e passa a ser null reference
            }
        }

        //i1[]
        for(unsigned i=0; i<N_INDIRECT; i++){
            if(pointer->i1[i] != BlockNullReference){ //verifica se está vazio
                bool null = false;
                if((int)i == i1Inicio){ 
                    null = grpFreeIndirectFileBlocks(pointer, pointer->i1[i], (ffbn-N_DIRECT)%RPB); 
                }else if((int)i > i1Inicio){
                    null = grpFreeIndirectFileBlocks(pointer, pointer->i1[i], 0);
                }

                if(null){
                    soFreeDataBlock(pointer->i1[i]);
                    freeFB++;
                    pointer->i1[i] = BlockNullReference;
                }

            }
        }

        //i2[]
        for(unsigned i=0; i<N_DOUBLE_INDIRECT; i++){
            if(pointer->i2[i] != BlockNullReference){
                bool null = false;
                if((int)i == i2Inicio){
                    null = grpFreeDoubleIndirectFileBlocks(pointer, pointer->i2[i], (ffbn-N_DIRECT-N_INDIRECT*RPB)%(RPB*RPB));
                }else if((int)i > i2Inicio){
                    null = grpFreeDoubleIndirectFileBlocks(pointer, pointer->i2[i], 0);
                }

                if(null){
                    soFreeDataBlock(pointer->i2[i]);
                    freeFB++;
                    pointer->i2[i]=BlockNullReference;
                }
            }
        }

        pointer->blkcnt -= freeFB; //block count é decrementado sempre que um file block é libertado

        soSaveInode(ih);

    }

    /* ********************************************************* */

//#if false
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        /* replace the following line with your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        uint32_t ref[RPB];
        soReadDataBlock(i1, ref);

        bool fullNull = true; //todas as referencias sao nulas
        for(unsigned i=0; i<RPB; i++){
            if(i>=ffbn && ref[i] != BlockNullReference){
                soFreeDataBlock(ref[i]);
                freeFB++;
                ref[i] = BlockNullReference;
            }
            if(ref[i] != BlockNullReference){
                fullNull = false;
            }
        }
        
        soWriteDataBlock(i1, ref);

        return fullNull; //true or false

    }
//#endif

    /* ********************************************************* */

//#if false
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* replace the following line with your code */
        //throw SOException(ENOSYS, __FUNCTION__); 

        uint32_t ref[RPB];
        soReadDataBlock(i2, ref);

        bool fullNull = true;

        for(unsigned i=0; i<RPB; i++){
            if(ref[i] != BlockNullReference){
                bool null = false;
                if(i==ffbn/RPB){
                    null = grpFreeIndirectFileBlocks(ip, ref[i], ffbn%RPB);
                }else if(i>ffbn/RPB){
                    null = grpFreeIndirectFileBlocks(ip, ref[i], 0);
                }

                if(null){
                    soFreeDataBlock(ref[i]);
                    freeFB++;
                    ref[i] = BlockNullReference;
                }

                if(ref[i] != BlockNullReference){
                    fullNull = false;
                }
            }
        }

        soWriteDataBlock(i2, ref);

        return fullNull;

    }
//#endif

    /* ********************************************************* */
};

