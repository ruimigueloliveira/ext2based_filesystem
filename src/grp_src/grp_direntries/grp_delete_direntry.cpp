#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs20
{
    uint16_t grpDeleteDirentry(int pih, const char *name)
    {
        soProbe(203, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        /* replace the following line with your code */
        //return binDeleteDirentry(pih, name);

        SOInode* inode_pointer = soGetInodePointer(pih);

        SODirentry dir_block[DPB];
        uint32_t block, dir, deleted;
        bool del = false;
        uint32_t lst_block = 0;
        uint32_t lst_entry = 0;
        uint32_t curr_block = 0;
        uint32_t curr_entry = 0;


        for (block = 0; block <= (*inode_pointer).size/BlockSize; block++) {

            sofs20::soReadFileBlock(pih, block, dir_block); //Lê o bloco

            for (dir = 0; dir < DPB; dir++) {
                
                if(del){
                    if(strcmp(dir_block[dir].name, "") == 0){
    
                        SODirentry current_dir_block[DPB];
                        //Troca a ultima entry pela apagada
                        if(lst_block == curr_block){
                            //Troca dentro do mesmo bloco
                            if(curr_entry == 0 && lst_entry == 0){
                                // Se o ultimo datablock ficar vazio, deve de ser libertado
                                soFreeFileBlocks(pih, curr_block);
                            }else{
                                soReadFileBlock(pih, curr_block, current_dir_block);
                                current_dir_block[curr_entry].in = current_dir_block[lst_entry].in;
                                strcpy(current_dir_block[curr_entry].name, current_dir_block[lst_entry].name);
                                memset(current_dir_block[lst_entry].name, '\0', SOFS20_FILENAME_LEN);
                                current_dir_block[lst_entry].in = 0;
                                //Atualiza o bloco com a entry apagada
                                soWriteFileBlock(pih, curr_block, current_dir_block);
                            }
                        }else{
                            //Troca de diferentes blocos
                            SODirentry last_dir_block[DPB];
                            soReadFileBlock(pih, curr_block, current_dir_block);
                            soReadFileBlock(pih, lst_block, last_dir_block);
                            current_dir_block[curr_entry].in = last_dir_block[lst_entry].in;
                            strcpy(current_dir_block[curr_entry].name, last_dir_block[lst_entry].name);
                            memset(last_dir_block[lst_entry].name, '\0', SOFS20_FILENAME_LEN);
                            last_dir_block[lst_entry].in = 0;
                            //Atualiza o bloco cuja entry foi apagada
                            soWriteFileBlock(pih, curr_block, current_dir_block);
                            if(lst_entry == 0){
                                //Se o ultimo bloco ficar vazio deve ser libertado
                                soFreeFileBlocks(pih, lst_block);
                            }else{
                                //Atualiza o ultimo bloco
                                soWriteFileBlock(pih,lst_block, last_dir_block);
                            }
                        }
                        return deleted;
                    }
                    else{
                        // Se a entry não é a ultima, a ultima deve de ser movida para o buraco
                        lst_block = block;
                        lst_entry = dir;
                    }
                }
                else{
                    if (strcmp(dir_block[dir].name, name) == 0) {
                        //Apaga a direntry
                        deleted = dir_block[dir].in;
                        memset(dir_block[dir].name, '\0', SOFS20_FILENAME_LEN);
                        dir_block[dir].in = 0;

                        //Escreve o bloco sem o valor apagado
                        sofs20::soWriteFileBlock(pih, block, dir_block);
                        inode_pointer->size -= sizeof(SODirentry);
                        del = true;
                        curr_block = block;
                        curr_entry = dir;
                        lst_block = block;
                        lst_entry = dir;
                    }
                }
                
            }

        }
        throw SOException(ENOENT, __FUNCTION__);
    }
};

