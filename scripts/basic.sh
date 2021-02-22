#!/bin/bash
# --------------------------------------------------------------------------

# Create the $SOFS20_DISK disk with the given number of blocks (default 1000)
function c() 
{ 
    local sz;
    if [ $# -eq 0 ]; then
        sz=1000
    else
        sz=$1
    fi;
    local cwd="$(pwd)"
    cd "$SOFS20_ROOT/bin"
    ./createDisk "$SOFS20_DISK" $sz
    local ret=$?
    cd $cwd
    return $ret
}

# --------------------------------------------------------------------------

# Call mksofs on $SOFS20_DISK disk
# By default it uses group version and maximum probing
# format
function f()
{
    local cwd="$(pwd)"
    cd "$SOFS20_ROOT/bin"
    ./mksofs -g -p 0-999 "$SOFS20_DISK" $@
    local ret=$?
    cd $cwd
    return $ret
}

# --------------------------------------------------------------------------

# Call showblock on $SOFS20_DISK disk
function s()
{
    local cwd="$(pwd)"
    cd "$SOFS20_ROOT/bin"
    ./showblock "$SOFS20_DISK" $@
    local ret=$?
    cd $cwd
    return $ret
}

# --------------------------------------------------------------------------

# Call testtool on $SOFS20_DISK disk
# By default it uses group version and maximum probing
function tt()
{
    local cwd="$(pwd)"
    cd "$SOFS20_ROOT/bin"
    ./testtool -g  -p 0-999 "$SOFS20_DISK" $@
    local ret=$?
    cd $cwd
    return $ret
}

# --------------------------------------------------------------------------

# Call make/ninja on build folder
function m()
{
    # call make
    local cwd=$(pwd)
    cd "$SOFS20_ROOT/build"
    if [ -f Makefile ]; then
        make $@
    elif [ -f rules.ninja ]; then
        ninja $@
    else
        WarnMessage "cmake needs to be configured first"
    fi
    cd "$cwd"
}

# --------------------------------------------------------------------------
# To test the deplete insertion cache function
function tt-dic()
{
    for i in $(seq 1 68)
    do
        echo -ne "adb\nq\n" | tt -q1 -p0-0 -b
    done

    for db in $(seq 1 68)
    do
        echo -ne "fdb\n$db\nq\ne" | tt -q1 -p0-0 -b
    done

    echo -ne "dic\nq\n" | tt -q1 -p0-0 -b $@
}

#----------------------------------------------------------------------------
# To test the Replenish Retrieval cache
function tt-rrc()
{
    for i in $(seq 1 68)
    do
    	echo -ne "adb\nq\n" | tt -q1 -p0-0 -b
    done
    
    echo -ne "rrc\nq\n" | tt -q1 -p0-0 -b $@
}
# --------------------------------------------------------------------------
# To test the get file block function
function tt-gfb()
{
    echo -ne "ai\n1\n555\nq\ne" | tt -q1 -p0-0 -b

    for i in $(seq 0 4)
    do
        echo -ne "afb\n1\n$i\nq\ne" | tt -q1 -p0-0 -b 
    done

    for index in 260 516 772
    do
        echo -ne "afb\n1\n$index\nq\ne" | tt -q1 -p0-0 -b
    done

    # Calling get file block function
    # Direct zone
    for j in $(seq 0 3)
    do
        echo -ne "gfb\n1\n$j\nq\ne" | tt -q1 -p0-0 -b $@
    done

    # Indirect zone
    for k in 4 260 516
    do
        echo -ne "gfb\n1\n$k\nq\ne" | tt -q1 -p0-0 -b $@
    done

    # 2x Indirect zone
    for l in 772
    do
        echo -ne "gfb\n1\n$l\nq\ne" | tt -q1 -p0-0 -b $@
    done
}