#! /bin/bash

if [ -z "$1" ]; then
    sudo sshfs -o allow_other -o ssh_command="ssh -J root@192.168.1.10" zso@10.8.13.106:/home/zso/new_kernel vm
else
    sudo sshfs -o allow_other -o ssh_command="ssh -J root@192.168.1.10" zso@$1:/home/zso/new_kernel vm
fi
