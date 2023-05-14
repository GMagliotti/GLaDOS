qemu-system-x86_64 -s -S -audiodev pa,id=foo -hda Image/x64BareBonesImage.qcow2 -m 512 -machine pcspk-audiodev=foo -d int
