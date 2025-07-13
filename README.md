# Single-Cycle CPU Simulator (RISC-V RV32I)

---

Core TO-DO

- [ ] Test and implement rest of loads/stores
- [ ] Add PC update on exec functions
- [ ] S-TYPE OFFSET EXTRACTION (1)

---

- [ ] In the CPU class, update accessibility of all fns and vars (defaulted to public for now)
- [ ] Block write ops on reg x0
- [ ] Implement non-core instructions (fence, etc)

Footnotes:

(1)
else if (opcode == STYPE_OPCODE) {
i.offset = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
i.offset = static_cast<int32_t>(i.offset << 20) >> 20; // Sign extend
}
