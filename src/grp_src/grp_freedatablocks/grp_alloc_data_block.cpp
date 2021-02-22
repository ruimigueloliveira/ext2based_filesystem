/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2009-2020
 */

#include "freedatablocks.h"

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
    uint32_t grpAllocDataBlock()
    {
        soProbe(441, "%s()\n", __FUNCTION__);

        /* replace the following line with your code */
        //return binAllocDataBlock();

        SOSuperblock* sagres =  soGetSuperblockPointer(); //ponteiro que fica com toda a informação do sb



        if(sagres -> retrieval_cache.idx == REF_CACHE_SIZE){ //se a retrieval cache estiver vazia, isto é, se o índice da primeira posição ocupada for igual ao tamanho da cache (68)
            soReplenishRetrievalCache(); //chama esta função, que vai á RT buscar as proximas 68 referencias e coloca-as na retrieval cache
        }

        uint32_t block;
        block = sagres -> retrieval_cache.ref[sagres -> retrieval_cache.idx]; //o bloco é o datablock que está na primeira posição ocupada (retrieval.idx) do array de elementos que estão na retrieval cache(ref[idx])
        sagres -> retrieval_cache.ref[sagres -> retrieval_cache.idx] = BlockNullReference; //a posição onde estava o datablock passa a ser nil

        sagres -> dbfree -= 1;  //decrementa o numero de free datablocks
        sagres -> retrieval_cache.idx += 1; //incrementa o indice do proximo bloco a ser alocado
        
        //ENOSPC if there are no free data blocks
        if(sagres -> dbfree == 0){
            throw SOException(ENOSPC, __FUNCTION__);
        }

        soSaveSuperblock(); //update do superblock
        return block;

    }
};