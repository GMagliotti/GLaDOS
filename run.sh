#!/bin/bash
qemu-system-x86_64 -audiodev pa,id=foo -hda Image/x64BareBonesImage.qcow2 -m 512
