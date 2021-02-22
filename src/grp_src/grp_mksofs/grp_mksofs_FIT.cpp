#include "grp_mksofs.h"
#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>

namespace sofs20
{
    void grpFillInodeTable(uint32_t itotal, bool date)
    {
        soProbe(604, "%s(%u)\n", __FUNCTION__, itotal);

        /* replace the following line with your code */
        /* binFillInodeTable(itotal, date); */

        SOInode inodes[IPB];

        for (unsigned long i = 0; i < (itotal / IPB); i++)
        {
            for (unsigned long j = 0; j < IPB; j++)
            {
                if (i == 0 && j == 0)
                {
                    inodes[j].mode = S_IFDIR | 0775;
                    inodes[j].lnkcnt = 2;
                    inodes[j].owner = getuid();
                    inodes[j].group = getgid();
                    inodes[j].size = BlockSize;
                    inodes[j].blkcnt = 1;
                    inodes[j].atime = time(NULL);
                    inodes[j].mtime = time(NULL);
                    inodes[j].ctime = time(NULL);
                    inodes[j].d[0] = 0;
                    for (int k = 1; k < N_DIRECT; k++)
                    {
                        inodes[j].d[k] = BlockNullReference;
                    }
                    for (int k = 0; k < N_INDIRECT; k++)
                    {
                        inodes[j].i1[k] = BlockNullReference;
                    }
                    for (int k = 0; k < N_DOUBLE_INDIRECT; k++)
                    {
                        inodes[j].i2[k] = BlockNullReference;
                    }
                }
                
                else
                {
                    inodes[j].mode = 0;
                    inodes[j].lnkcnt = 0;
                    inodes[j].owner = 0;
                    inodes[j].group = 0;
                    inodes[j].size = 0;
                    inodes[j].blkcnt = 0;
                    inodes[j].atime = 0;
                    inodes[j].ctime = 0;
                    inodes[j].mtime = 0;
                    for (int k = 0; k < N_DIRECT; k++)
                    {
                        inodes[j].d[k] = BlockNullReference;
                    }
                    for (int k = 0; k < N_INDIRECT; k++)
                    {
                        inodes[j].i1[k] = BlockNullReference;
                    }
                    for (int k = 0; k < N_DOUBLE_INDIRECT; k++)
                    {
                        inodes[j].i2[k] = BlockNullReference;
                    }
                }
            }
            
            soWriteRawBlock(i + 1, inodes);
        }
    }
};