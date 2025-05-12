#include "simulator/memory_manager.h"
#include "simulator/cache.h"

#include <cstdio>
#include <string>


MemoryManager::MemoryManager() {
    this->cache = nullptr;
    for (uint32_t i = 0; i < 1024; ++i) {
        memory[i] = nullptr;
    }
    access_latency = 100;
}

MemoryManager::~MemoryManager() {
    for (uint32_t i = 0; i < 1024; ++i) {
        if (memory[i] != nullptr) {
            for (uint32_t j = 0; j < 1024; ++j) {
                if (memory[i][j] != nullptr) {
                    delete[] memory[i][j];
                    memory[i][j] = nullptr;
                }
            }
            delete[] memory[i];
            memory[i] = nullptr;
        }
    }
}

uint32_t MemoryManager::getFirstEntryId(uint32_t addr) {
  return (addr >> 22) & 0x3FF;
}

uint32_t MemoryManager::getSecondEntryId(uint32_t addr) {
  return (addr >> 12) & 0x3FF;
}

uint32_t MemoryManager::getPageOffset(uint32_t addr) { 
  return addr & 0xFFF; 
  }

bool MemoryManager::addPage(uint32_t addr) {
  uint32_t i = getFirstEntryId(addr);
  uint32_t j = getSecondEntryId(addr);
  if (memory[i] == nullptr) {
    memory[i] = new uint8_t *[1024];
    memset(memory[i], 0, sizeof(uint8_t *) * 1024);
  }
  if (memory[i][j] == nullptr) {
    memory[i][j] = new uint8_t[4096];
    memset(memory[i][j], 0, 4096);
  } else {
    return false;
  }
  return true;
}


bool MemoryManager::isAddrExist(uint32_t addr) {
  uint32_t i = getFirstEntryId(addr);
  uint32_t j = getSecondEntryId(addr);
  if (memory[i] && memory[i][j]) {
    return true;
  }
  return false;
}


bool MemoryManager::isPageExist(uint32_t addr) {
  return isAddrExist(addr);
}


bool MemoryManager::setByte(uint32_t addr, uint8_t val, uint32_t *cycles) {
    if (!isAddrExist(addr))
        throw std::runtime_error("Memory address not found set byte");
    
    if (cycles != nullptr)
        *cycles = access_latency;

    if (this->cache != nullptr) {
        this->cache->setByte(addr, val, cycles);
        return true;
    }

    memory[getFirstEntryId(addr)][getSecondEntryId(addr)]
    [getPageOffset(addr)] = val;
    return true;
}

bool MemoryManager::setByteNoCache(uint32_t addr, uint8_t val) {
  if (!isAddrExist(addr)) {
    throw std::runtime_error("Memory address not found");
  }

      memory[getFirstEntryId(addr)][getSecondEntryId(addr)]
    [getPageOffset(addr)] = val;
    return true;
  return true;
}

uint8_t MemoryManager::getByte(uint32_t addr, uint32_t *cycles) {
  if (!isAddrExist(addr)) {
    throw std::runtime_error("1Memory address not found");
  }
  if (cycles != nullptr)
        *cycles = access_latency;

  if (this->cache != nullptr) {
    return this->cache->getByte(addr, cycles);
  }
      return memory[getFirstEntryId(addr)][getSecondEntryId(addr)]
    [getPageOffset(addr)];
}

uint8_t MemoryManager::getByteNoCache(uint32_t addr) {
  if (!isAddrExist(addr)) {
    throw std::runtime_error("2Memory address not found");
  }
      return memory[getFirstEntryId(addr)][getSecondEntryId(addr)]
    [getPageOffset(addr)];
}

bool MemoryManager::setShort(uint32_t addr, uint16_t val, uint32_t *cycles) {
  if (!isAddrExist(addr)) {
    throw std::runtime_error("Memory address not found");
  }
  setByte(addr, val & 0xFF, cycles);
  setByte(addr + 1, (val >> 8) & 0xFF);
  return true;
}

uint16_t MemoryManager::getShort(uint32_t addr, uint32_t *cycles) {
    if (!isAddrExist(addr)) {
        throw std::runtime_error("Memory address not found");
    }
    return getByte(addr, cycles) | getByte(addr + 1) << 8;
}

bool MemoryManager::setInt(uint32_t addr, uint32_t val, uint32_t *cycles) {
  if (!isAddrExist(addr)) {
    throw std::runtime_error("Memory address not found");
  }
  setShort(addr, val & 0xFFFF, cycles);
  setShort(addr + 2, (val >> 16) & 0xFFFF);
  return true;
}

uint32_t MemoryManager::getInt(uint32_t addr, uint32_t *cycles) {
    if (!isAddrExist(addr)) {
        throw std::runtime_error("Memory address not found");
    }
    return getShort(addr, cycles) | getShort(addr + 2) << 16;
}

bool MemoryManager::setLong(uint32_t addr, uint64_t val, uint32_t *cycles) {
  if (!this->isAddrExist(addr)) {
    throw std::runtime_error("Memory address not found");
    return false;
  }
  this->setByte(addr, val & 0xFF, cycles);
  this->setByte(addr + 1, (val >> 8) & 0xFF);
  this->setByte(addr + 2, (val >> 16) & 0xFF);
  this->setByte(addr + 3, (val >> 24) & 0xFF);
  this->setByte(addr + 4, (val >> 32) & 0xFF);
  this->setByte(addr + 5, (val >> 40) & 0xFF);
  this->setByte(addr + 6, (val >> 48) & 0xFF);
  this->setByte(addr + 7, (val >> 56) & 0xFF);
  return true;
}

uint64_t MemoryManager::getLong(uint32_t addr, uint32_t *cycles) {
  uint64_t b1 = this->getByte(addr, cycles);
  uint64_t b2 = this->getByte(addr + 1);
  uint64_t b3 = this->getByte(addr + 2);
  uint64_t b4 = this->getByte(addr + 3);
  uint64_t b5 = this->getByte(addr + 4);
  uint64_t b6 = this->getByte(addr + 5);
  uint64_t b7 = this->getByte(addr + 6);
  uint64_t b8 = this->getByte(addr + 7);
  return b1 + (b2 << 8) + (b3 << 16) + (b4 << 24) + (b5 << 32) + (b6 << 40) +
         (b7 << 48) + (b8 << 56);
}


void MemoryManager::setCache(Cache *cache) { this->cache = cache; }