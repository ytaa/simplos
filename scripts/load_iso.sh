#!/bin/bash
cd ../kernel/iso
sudo dd if=$DEVOS_NAME.iso of=$1 && sync

