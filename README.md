# Merkle Patricia tree
```bash
$ git clone git@github.com:Azazel-h/merkle-patricia-tree.git
$ git submodule update --init --recursive
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
> Be careful, if you use CLion or othe IDE with СMake and CTest integration,
> run the mpt-tests to build them before using CTest.
You can run tests with smth like
```bash
$ ./build/mpt-tests
```
Or using CTest
```bash
$ ctest --verbose -j$(nproc) --test-dir build
```