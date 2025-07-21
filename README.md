# Single-Cycle CPU Simulator (RISC-V RV32m)

---

- [ ] In the CPU class, update accessibility of all fns and vars (defaulted to public for now)
- [ ] Block write ops on reg x0
- [ ] Implement non-core instructions (fence, etc)

~~llvm-mc --arch="riscv32" --assemble --filetype=obj tests/nested_loop.so -o tests/nested_loop.o~~

~~objcopy -O binary tests/nested_loop.o tests/nested_loop.bin~~

How to run a file:
llvm-mc --arch="riscv32" -mattr="zmmul" -mattr="M" --assemble --filetype=obj tests_asm/mul.so -o tmp/mul.o

objcopy -O binary tmp/mul.o tests_x/mul.bin