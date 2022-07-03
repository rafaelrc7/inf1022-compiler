# Build Instructions

- [GNU Autotools](#gnu-autotools)
- [Nix](#nix)

## GNU Autotools
Works for any POSIX system.

### Dependencies
- Flex
- Bison
- GCC

### Instructions
```sh
mkdir build && cd build
../configure && make
./provolone --version
```

## Nix

```sh
nix build
./result/bin/provolone --version
```

