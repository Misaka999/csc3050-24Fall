/**
 * main entry point of 3-level cache
 */

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "simulator/cache.h"
#include "simulator/memory_manager.h"

bool parseParameters(int argc, char **argv);
void printUsage();

const char *traceFilePath;

int main(int argc, char **argv) {
  if (!parseParameters(argc, argv)) {
    return -1;
  }

  Cache::Policy l1policy, l2policy, l3policy, victimpolicy;
  victimpolicy.cacheSize = 8 * 64; 
  victimpolicy.blockSize = 64;
  victimpolicy.blockNum = 8;
  victimpolicy.associativity = 1;
  victimpolicy.hitLatency = 1;
  victimpolicy.missLatency = 8;

  l1policy.cacheSize = 16 * 1024; // 16 KB
  l1policy.blockSize = 64;
  l1policy.blockNum = l1policy.cacheSize / l1policy.blockSize;
  l1policy.associativity = 1;
  l1policy.hitLatency = 1;
  l1policy.missLatency = 1;

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

  // Initialize memory and caches
  MemoryManager *memory = nullptr;
  memory = new MemoryManager();
  Cache *l1cache = nullptr, *l2cache = nullptr, *l3cache = nullptr; 
  Cache *victimCache = nullptr;

  //inclusive
  l3cache = new Cache(memory, l3policy);
  l2cache = new Cache(memory, l2policy,l3cache);
  victimCache = new Cache(memory, victimpolicy, l2cache);
  l1cache = new Cache(memory, l1policy, l2cache, victimCache);
  memory->setCache(l1cache);

  // Read and execute trace in cache-trace/ folder
  std::ifstream trace(traceFilePath);
  if (!trace.is_open()) {
    printf("Unable to open file %s\n", traceFilePath);
    exit(-1);
  }

  char type; //'r' for read, 'w' for write
  uint32_t addr;
  while (trace >> type >> std::hex >> addr) {
    if (!memory->isPageExist(addr))
      memory->addPage(addr);
    switch (type) {
    case 'r':
      memory->getByte(addr);
      break;
    case 'w':
      memory->setByte(addr, 0);
      break;
    default:
      fprintf(stderr, "Illegal type %c\n", type);
      exit(-1);
    }
  }

  // Output Simulation Results
  printf("L1 Cache:\n");
  l1cache->printStatistics();
  printf("\n");
  printf("L2 Cache:\n");
  l2cache->printStatistics();
  printf("\n");
  printf("L3 Cache:\n");
  l3cache->printStatistics();
  printf("\n");
  printf("Victim Cache:\n");
  victimCache->printStatistics();

  delete l1cache;
  delete l2cache;
  delete l3cache;
  delete memory;
  return 0;
}

bool parseParameters(int argc, char **argv) {
  // Read Parameters
  if (argc > 1) {
    traceFilePath = argv[1];
    return true;
  } else {
    return false;
  }
}

void printUsage() { printf("Usage: CacheSim trace-file\n"); }