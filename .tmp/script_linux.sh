#!/bin/sh

export LD_LIBRARY_PATH="." 

export LD_PRELOAD="/home/ezalos/42/malloc/libft_malloc.so"

#export HUGETLB_MORECORE=yes

$@
