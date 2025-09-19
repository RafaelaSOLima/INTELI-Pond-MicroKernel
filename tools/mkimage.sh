#!/bin/bash
dd if=/dev/zero of="$3" bs=512 count=2880
dd if="$1" of="$3" bs=512 conv=notrunc
dd if="$2" of="$3" bs=512 seek=1 conv=notrunc