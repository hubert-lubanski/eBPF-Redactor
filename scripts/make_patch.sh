#!/bin/bash
cd /mnt/data/Studia/ZSO


prefix=$(date +%Y-%m-%d.%H:%M:%S)
PATCHFILE="patches/patch.$prefix.diff"

ORIGINAL="original-kernel"
CHANGED="kernel"
# ORIGINAL="vm/patch_test"
# CHANGED="patch_test"



_optimize="--speed-large-files"

echo "Creating patch file ($PATCHFILE)..."
diff -EZwBrpus $_optimize --unidirectional-new-file "$ORIGINAL" "$CHANGED" |
    tee >(grep --invert-match "^Pliki" > $PATCHFILE) >(grep -E "^(diff|Pliki)" | pv -l > /dev/null) > /dev/null;

if [ ! -z "$1" ]; then
    echo "Copying patch file to $1 ...";
    cp -v $PATCHFILE $1;
fi

if [ ! -z "$2" ]; then
    echo "Making intermediate file from $2 ...";

    if [ ! -z "$3" ]; then
        interdiff $2 $1 > $3;
    else
        interdiff $2 $1 > $(basename $2).interdiff
    fi
fi
        
        


