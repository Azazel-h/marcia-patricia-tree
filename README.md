# Merkle Patricia tree
![Testing](https://github.com/Azazel-h/merkle-patricia-tree/actions/workflows/cmake-multi-platform.yml/badge.svg) [![codecov](https://codecov.io/gh/Azazel-h/merkle-patricia-tree/graph/badge.svg?token=XJEFCWDOQK)](https://codecov.io/gh/Azazel-h/merkle-patricia-tree)
```bash
$ git clone https://github.com/Azazel-h/merkle-patricia-tree.git --recursive
```

Clean building
```bash
$ cmake -S . -B build
$ cmake --build build
```
> [!NOTE]
> If you want to build with tests just add the flag
> ```bash
> $ cmake -S . -B build -DTRIE_BUILD_TESTING=ON
> $ cmake --build build
> ```

> [!IMPORTANT]
> Be **careful**, if you are using CLion or other IDE with _СMake/CTest_ integration,
> run the **mpt-tests** to build them before using CTest.

You can run tests with smth like
```bash
$ ./build/mpt-tests
```
Or using CTest
```bash
$ ctest --verbose -j$(nproc) --test-dir build
```