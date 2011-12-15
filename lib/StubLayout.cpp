/*
 * Copyright 2011, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "StubLayout.h"

#include "utils/flush_cpu_cache.h"
#include "utils/raw_ostream.h"

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#ifdef __arm__
#define STUB_SIZE 8 // 8 bytes (2 words)
#elif defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)
#define STUB_SIZE 16 // 16 bytes (4 words)
#endif

StubLayout::StubLayout() : table(NULL), count(0) {
}

void StubLayout::initStubTable(unsigned char *table_, size_t count_) {
  table = table_;
  count = count_;
}

void *StubLayout::allocateStub(void *addr) {
#if defined(__arm__)\
    || defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)
  if (count == 0) {
    // No free stub slot is available
    return NULL;
  }

  // Initialize the stub
  unsigned char *stub = table;
  setStubAddress(stub, addr);

  // Increase the free stub slot pointer
  table += STUB_SIZE;
  count--;

  return stub;

#else
  return NULL;
#endif
}

void StubLayout::setStubAddress(void *stub_, void *addr) {
#ifdef __arm__
  uint8_t *stub = (uint8_t *)stub_;
  stub[0] = 0x04; // ldr pc, [pc, #-4]
  stub[1] = 0xf0; // ldr pc, [pc, #-4]
  stub[2] = 0x1f; // ldr pc, [pc, #-4]
  stub[3] = 0xe5; // ldr pc, [pc, #-4]

  void **target = (void **)(stub + 4);
  *target = addr;
#elif defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)
  // 0:   3c190000        lui     t9,%hi(addr)
  // 4:   27390000        addiu   t9,t9,%lo(addr)
  // 8:   03200008        jr      t9
  // c:   00000000        nop
  int *stub = (int *)stub_;
  *stub = 0x3c190000 | ((((int)addr + 0x8000) >> 16) & 0xFFFF);
  stub++;
  *stub = 0x27390000 | ((int)addr & 0xFFFF);
  stub++;
  *stub = 0x03200008;
  stub++;
  *stub = 0;
#endif
}

size_t StubLayout::calcStubTableSize(size_t count) const {
#if defined(__arm__)\
    || defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)
  return count * STUB_SIZE;
#else
  return 0;
#endif
}
