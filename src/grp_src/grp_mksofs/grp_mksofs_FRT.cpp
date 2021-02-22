#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <string.h>

namespace sofs20
{
    void grpFillReferenceTable(uint32_t ntotal, uint32_t itotal, uint32_t dbtotal)
    {
        soProbe(605, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, dbtotal);

        /* replace the following line with your code */
        //binFillReferenceTable(ntotal, itotal, dbtotal);
	
	uint32_t arrayRef[RPB]; //array de referencias
	uint32_t firstRefTableBlockIndex = 1 + (itotal/IPB) + dbtotal; //#superBlock + #inodeTable + #dataBlockPool 
	uint32_t firstEmptyDB = REF_CACHE_SIZE + 1; //valor da primeira referência
	uint32_t dbfree = dbtotal; //numero de free data blocks

	for(uint32_t i=firstRefTableBlockIndex; i<ntotal; i++){ //vai do primeiro bloco do referenceTable até ao ultimo
		
		for(uint32_t j=0; j<RPB; j++){ //vai de 0 a 254

			arrayRef[j] = firstEmptyDB++; //cada posição fica com os valores sucessivos a partir de 68
				
			if(firstEmptyDB > dbfree){ //verifica se o numero de referências é superior ao numero de free datablocks
				arrayRef[j] = BlockNullReference; //coloca a null as referencias q apontam para data blocks ocupados
			}

		}

		soWriteRawBlock(i, arrayRef); //escreve o array em cada bloco da refTable
	}

	};

};

