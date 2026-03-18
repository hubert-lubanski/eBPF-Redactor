#!/bin/bash

cd "$(dirname "$0")/.."

echo "Mounting original-kernel as read-only ..."
sudo mount -o ro --bind .original-kernel original-kernel || exit 1

echo "Mounting virtual machine directory ..."
sudo sshfs -o allow_other -o ProxyJump=root@10.8.12.1 zso@10.8.13.99:/home/zso/new_kernel vm || exit 1

source ./scripts/activate.sh