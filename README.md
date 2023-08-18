# poor perfomance lib :(

currently i use SDL2 to draw pixels, i'll get rid of it later (i hope)

O(n^inf) 😋

## Requirements

### openSUSE Tumbleweed 
```bash
sudo zypper in libSDL2-2_0-0 SDL2-devel SDL2_ttf-devel
```

### Fedora 36
```bash
sudo dnf install SDL2 SDL2-devel SDL2_ttf SDL2_ttf-devel
```

### Elbrus Linux 7.2
```bash
mkdir SDL2
wget https://github.com/libsdl-org/SDL/releases/download/release-2.28.2/SDL2-2.28.2.tar.gz
tar zxf SDL2-2.28.2.tar.gz
cd SDL2-2.28.2
./configure --prefix="$(pwd)/../SDL2"
make -j$(nproc)
make install
cd ../
make -f Makefile.linux_e2k
LD_LIBRARY_PATH="./SDL2/lib/:$LD_LIBRARY_PATH" ./out
```
