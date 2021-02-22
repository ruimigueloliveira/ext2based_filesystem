#!/bin/bash
# --------------------------------------------------------------------------

function tthelp()
{
    helpmsg="List of bash testtool tools:\n"
        helpmsg+="  ttai    --- Alloc inode on disk $SOFS20_DISK\n"
        helpmsg+="  ttfi    --- Free inode on disk $SOFS20_DISK\n"
        helpmsg+="  ttadb   --- Alloc data block on disk $SOFS20_DISK\n"
        helpmsg+="  ttfdb   --- Free data block on disk $SOFS20_DISK\n"
        helpmsg+="  ttrrc   --- Replenish retrieval cache on disk $SOFS20_DISK\n"
        helpmsg+="  ttdic   --- Deplete insertion cache on disk $SOFS20_DISK\n"
        helpmsg+="  ttgfb   --- Get file block on disk $SOFS20_DISK\n"
        helpmsg+="  ttafb   --- Alloc file block on disk $SOFS20_DISK\n"
        helpmsg+="  ttffb   --- Free file blocks on disk $SOFS20_DISK\n"
        helpmsg+="  ttrfb   --- Read file block on disk $SOFS20_DISK\n"
        helpmsg+="  ttwfb   --- Write file block on disk $SOFS20_DISK\n"
        helpmsg+="  ttgde   --- Get direntry on disk $SOFS20_DISK\n"
        helpmsg+="  ttade   --- Add direntry on disk $SOFS20_DISK\n"
        helpmsg+="  ttdde   --- Delete direntry on disk $SOFS20_DISK\n"
        helpmsg+="  ttrde   --- Rename direntry on disk $SOFS20_DISK\n"
        helpmsg+="  ttcde   --- Check directory emptiness on disk $SOFS20_DISK\n"
        helpmsg+="  tttp    --- Traverse path on disk $SOFS20_DISK\n"

        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttai()
{
    helpmsg="ttai «type» ... [ OPTIONS ]\n"
        helpmsg+="  Alloc 1 or more inodes\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «type»       --- inode type (1 - file, 2 - dir, 3 - link)\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

    local list="" 
    local ttoptions="-g" verbose=0
    local permissions
    while [[ $# -gt 0 ]]
    do
        case $1 in 
            "1"|"2"|"3") # inode types
                list+=" $1" # add to list
                shift 1
                ;;
            "-b"|"-g") # testtool options are the same
                ttoptions+=" $1"
                shift 1
                ;;
            "-p") # probe only this function
                ttoptions+=" -p 401"
                verbose=1
                shift 1
                ;;
            "-v") # probe all functions
                ttoptions+=" -p 0-999"
                verbose=1
                shift 1
                ;;
            "-h") # help message
                InfoMessage "$helpmsg"
                return
                ;;
            *) # some thing wrong
                ErrorMessage "Wrong arguments: \"$@\""
                InfoMessage "$helpmsg"
                break
                ;;
        esac
    done

    for t in $list
    do
        case $t in
            1) permissions=644;;
            2) permissions=755;;
            3) permissions=777;;
        esac
        if [ $verbose -eq 1 ]; then
            echo -ne "ai\n$t\n$permissions\nq\n" | tt -q 1 $ttoptions
        else
            echo -ne "ai\n$t\n$permissions\nq\n" | tt -q 1 $ttoptions -p 0-0 | head -1 | cut -d\  -f3
        fi
    done
        
#        WarnMessage "NOT IMPLEMENTED YET"
#        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttfi()
{
    helpmsg="ttfi «num» ... [ OPTIONS ]\n"
        helpmsg+="  Free 1 or more inodes on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- inode number\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttadb()
{
    helpmsg="ttadb [ OPTIONS ]\n"
        helpmsg+="  Alloc data block on disk $SOFS20_DISK\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttfdb()
{
    helpmsg="ttfdb «num» ... [ OPTIONS ]\n"
        helpmsg+="  Free 1 or more data blocks on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- data block number\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttrrc()
{
    helpmsg="ttrrc [ OPTIONS ]\n"
        helpmsg+="  Replenish retrieval cache on disk $SOFS20_DISK\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
        
        for i in $(seq 1 68)
    do
    	echo -ne "adb\nq\n" | tt -q1 -p0-0 -b
    done
    
    echo -ne "rrc\nq\n" | tt -q1 -p0-0 -b $@
}

# --------------------------------------------------------------------------

function ttdic()
{
    helpmsg="ttdic [ OPTIONS ]\n"
        helpmsg+="  Deplete insertion cache on disk $SOFS20_DISK\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttgfb()
{
    helpmsg="ttgfb «num» [ OPTIONS ]\n"
        helpmsg+="  Get data block of a given file block position on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- file block position\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -in «num»    --- inode number (default: 1)\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttafb()
{
    helpmsg="ttafb «num» [ OPTIONS ]\n"
        helpmsg+="  Alloc data block to a given file block position on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- file block position\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -in «num»    --- inode number (default: 1)\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttffb()
{
    helpmsg="ttffb «num» [ OPTIONS ]\n"
        helpmsg+="  Free data blocks from a given file blocks position to the end on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- file block position\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -in «num»    --- inode number (default: 1)\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttrfb()
{
    helpmsg="ttrfb «num» [ OPTIONS ]\n"
        helpmsg+="  Read data block of a given file block position on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- file block position\n"
        helpmsg+="  \n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -in «num»    --- inode number (default: 1)\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttwfb()
{
    helpmsg="ttwfb «num» [ OPTIONS ]\n"
        helpmsg+="  Write data block of a given file block position on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «num»        --- file block position\n"
        helpmsg+="  \n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -in «num»    --- inode number (default: 1)\n"
        helpmsg+="  -pt «patt»   --- byte pattern, in hexa, to write (default: 33)\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttgde()
{
    helpmsg="ttgde «pin» «name» [ OPTIONS ]\n"
        helpmsg+="  Search and return the inode number of a given name in a given direntry on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «pin»        --- number of the inode (directory) where to search\n"
        helpmsg+="  «name»       --- name to search for\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttade()
{
    helpmsg="ttade «pin» «name» «cin» [ OPTIONS ]\n"
        helpmsg+="  Add a new direntry on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «pin»        --- number of the inode (directory) where to add\n"
        helpmsg+="  «name»       --- name to be added\n"
        helpmsg+="  «cin»        --- inode number to be added\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttdde()
{
    helpmsg="ttdde «pin» «name» [ OPTIONS ]\n"
        helpmsg+="  Delete the entry with name «name» and return the associated inode on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «pin»        --- number of the inode (directory) where to delete\n"
        helpmsg+="  «name»       --- name to be deleted\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttrde()
{
    helpmsg="ttrde «pin» «name» «newname» [ OPTIONS ]\n"
        helpmsg+="  Rename entry with name «name» to «newname» on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «pin»        --- number of the inode (directory) where to rename\n"
        helpmsg+="  «name»       --- name to be renamed\n"
        helpmsg+="  «newname»    --- new name for the entry\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function ttcde()
{
    helpmsg="ttcde «pin» [ OPTIONS ]\n"
        helpmsg+="  Check if a given directory (inode) is empty on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «pin»        --- number of the inode (directory) to check\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

function tttp()
{
    helpmsg="tttp «path» [ OPTIONS ]\n"
        helpmsg+="  Return the inode associated to a given absolute path on disk $SOFS20_DISK\n"
        helpmsg+="PARAMETERS:\n"
        helpmsg+="  «path»       --- absolute path to traverse\n"
        helpmsg+="OPTIONS:\n"
        helpmsg+="  -b           --- use binary version\n"
        helpmsg+="  -g           --- use group version\n"
        helpmsg+="  -p           --- probe this function\n"
        helpmsg+="  -v           --- probe all functions\n"
        helpmsg+="  -h           --- this help"

        WarnMessage "NOT IMPLEMENTED YET"
        InfoMessage "$helpmsg"
}

# --------------------------------------------------------------------------

