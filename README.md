# OS Project
## Cloning quemu

```
git clone https://gitlab.com/qemu-project/qemu.git
```

Dependiences
```
sudo pacman -S edk2-ovmf
```
## Configure quemu build

```
./configure --target-list="x86_64-softmmu" --enable-debug  --extra-cflags="-Wno-error=redundant-decls"
```

## Building quemu

```
cd build && make -j8
```

## Run quemu

```
./build/qemu-system-x86_64 \
                    -m 512M \
                    -bios /usr/share/edk2-ovmf/x64/OVMF.4m.fd \
                    -drive file=fat:rw:image,format=raw \
                    -device AREK \
                    -monitor stdio
```
