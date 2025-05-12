#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <elfio/elfio.hpp>

#include "simulator/cache.h"
#include "simulator/memory_manager.h"
#include "simulator/simulator.h"

bool parse_params(int argc, char **argv);
void loadElfToMemory(ELFIO::elfio *reader, MemoryManager *memory);

char *elf_file = nullptr;
bool use_cache = 0;   // if with cach

uint32_t stackBaseAddr = 0x80000000;
uint32_t stackSize = 0x400000;
MemoryManager memory;
Cache *l1Cache, *l2Cache, *l3Cache;

Simulator simulator(&memory);

/*
 * Main function for a simulator that operates with ELF files.
 * Call parse_params and load_elf_to_memory function.
 * If the parameters are valid, it attempts to load the specified
 * ELF file using the ELFIO library.
 * Upon successful loading, transfers the ELF file content
 * into the simulator's memory.
 * Then sets up the simulator with the parsed configurations,
 * initializes the stack, and begins the simulation process.
 * If any step fails, the program exits with an error.
 */
int main(int argc, char **argv) {
  if (!parse_params(argc, argv)) {
    exit(-1);
  }

  // Init cache
  if (use_cache){
    Cache::Policy l1policy, l2policy, l3policy;

    l1policy.cacheSize = 16 * 1024; // 16 KB
    l1policy.blockSize = 64;
    l1policy.blockNum = l1policy.cacheSize / l1policy.blockSize;
    l1policy.associativity = 1;
    l1policy.hitLatency = 1;
    l1policy.missLatency = 8;

    l2policy.cacheSize = 128 * 1024;  // 128 KB
    l2policy.blockSize = 64;
    l2policy.blockNum = l2policy.cacheSize / l2policy.blockSize;
    l2policy.associativity = 8;
    l2policy.hitLatency = 8;
    l2policy.missLatency = 20;

    l3policy.cacheSize = 2 * 1024 * 1024;  // 2 MB
    l3policy.blockSize = 64;
    l3policy.blockNum = l3policy.cacheSize / l3policy.blockSize;
    l3policy.associativity = 16;
    l3policy.hitLatency = 20;
    l3policy.missLatency = 100; 

    l3Cache = new Cache(&memory, l3policy);
    l2Cache = new Cache(&memory, l2policy, l3Cache);
    l1Cache = new Cache(&memory, l1policy, l2Cache);

    memory.setCache(l1Cache);
  }

  // Read ELF file
  elf_file = argv[1];
  ELFIO::elfio reader;
  if (!reader.load(elf_file)) {
    fprintf(stderr, "Fail to load ELF file %s!\n", elf_file);
    return -1;
  }

  loadElfToMemory(&reader, &memory);

  simulator.use_cache = use_cache;
  simulator.pc = reader.get_entry();

  simulator.initStack(stackBaseAddr, stackSize);
  simulator.simulate();

  if (use_cache){
    delete l1Cache;
    delete l2Cache;
    delete l3Cache;
  }
  return 0;
}

/*
 * Processes command-line argument.
 * information should be printed upon program completion.
 * Any unrecognized option returns false.
 * The first non-option argument is expected to be the ELF file name for simulation.
 * If no ELF file is specified, returns false.
 */
bool parse_params(int argc, char** argv) {
    // read parameters implementation
    for (int i = 1; i < argc; ++i) {
       std::string arg = argv[i];
       if (arg[0] == '-') {
           if (arg == "-c") {
               use_cache = true;
           }
           else {
               fprintf(stderr, "Unknown option! \n");
               return false;
           }
       }
       else {
          if (elf_file == nullptr) {
            elf_file = argv[i];
          }
          else return false;
       }
    }
    if (elf_file == nullptr) {
      return false;
    }
    return true;
}


/*
 * Loads an ELF file into the simulator's memory.
 * Iterates through each segment in the ELF file,
 * using the ELFIO library for accessing ELF file information.
 * For each segment, it checks if its virtual address space
 * exceeds the 32-bit limit, indicating an error.
 * Then copies the segment's data into the simulator's memory,
 * allocates pages if they do not exist.
 * Data is copied up to the segment's file size,
 * any additional space up to the memory size is zero-initialized.
*/
void loadElfToMemory(ELFIO::elfio *reader, MemoryManager *memory) {
    ELFIO::Elf_Half seg_num = reader->segments.size();
    for (int i = 0; i < seg_num; ++i) {
        const ELFIO::segment* pseg = reader->segments[i];

        uint32_t memory_size = pseg->get_memory_size();
        uint32_t offset = pseg->get_virtual_address();

        // check if the address space is larger than 32bit
        if (offset + memory_size > 0xFFFFFFFF) {
            fprintf(
                    stderr,
                    "ELF address space larger than 32bit! Seg %d has max addr of 0x%x\n",
                    i, offset + memory_size);
            exit(-1);
        }

        uint32_t filesz = pseg->get_file_size();
        uint32_t memsz = pseg->get_memory_size();
        uint32_t addr = (uint32_t)pseg->get_virtual_address();

        for (uint32_t p = addr; p < addr + memsz; ++p) {
            if (!memory->isPageExist(p)) {
                memory->addPage(p);
            }

            if (p < addr + filesz) {
                memory->setByteNoCache(p, pseg->get_data()[p - addr]);
            } else {
                memory->setByteNoCache(p, 0);
            }
        }
    }
}