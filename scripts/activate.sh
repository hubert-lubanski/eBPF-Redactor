#! /bin/bash

breaklink()
{
if [ -z "$1" ]; then
    echo "Please provide a link to break.";
    return 1;
fi;

current_dir=$(pwd);
break_file="$(pwd)/$1";
original_file=$(readlink $break_file)

# echo "Current: $current_dir";
# echo "Break: $break_file"
# echo "Original: $original_file"

if [ -z $original_file ]; then
    echo "Selected file is not a link!";
    return 1;
fi

if [ -f $original_file ]; then
    echo "Selected link to a file, copying it ..."

    rm $break_file;
    cp -av --remove-destination $original_file $break_file;
elif [ -d $original_file ]; then
    echo "Selected link to a directory, breaking it ..."

    rm $break_file;
    mkdir $break_file;
    
    pushd $break_file > /dev/null;
    path=$original_file
    files=$(ls -A $original_file/)
    for i in $files; do
        echo "Creating a link $i -> $path/$i"
        ln -s "$path/$i" $i;
    done
    popd > /dev/null;
fi;

return 0;
}

alias lsreal="find . -mindepth 1 ! -type l";






    