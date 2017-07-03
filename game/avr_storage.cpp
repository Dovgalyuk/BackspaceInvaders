#pragma GCC optimize ("-O3")

#ifndef EMULATED

#define STORAGE_SIZE 1024
#define SECTOR_SIZE 12
#define DATA_SIZE (SECTOR_SIZE - 1)
#define SECTOR_OFFSET 4
#define SECTORS 85
#define FILENAME_LENGTH 10

#define STORAGE_SIG0 'B'
#define STORAGE_SIG1 's'
#define STORAGE_SIG2 'I'

#define STORAGE_CLOSED 0
#define STORAGE_READ 1
#define STORAGE_WRITE 2
#define STORAGE_FAILED 0xff
#define STORAGE_EOF 0xff

#define MAX_OPEN_FILES 1

#include "storage.h"
#include <avr/eeprom.h>
#include <string.h>

static uint8_t EEPROM_read(int addr)
{
    return eeprom_read_byte((uint8_t*)addr);
}

static void EEPROM_read_buffer(int addr, void *buf, uint8_t size)
{
    eeprom_read_block((uint8_t*)buf, (uint8_t*)addr, size);
}

static void EEPROM_update_buffer(int addr, const void *buf, uint8_t size)
{
    eeprom_update_block((uint8_t*)buf, (uint8_t*)addr, size);
}

static void EEPROM_update(int addr, uint8_t value)
{
    eeprom_update_byte((uint8_t*)addr, value);
}

static uint8_t storage_get_first_entry()
{
    return EEPROM_read(3);
}

void storage_init()
{
    if ((EEPROM_read(0) != STORAGE_SIG0) ||
        (EEPROM_read(1) != STORAGE_SIG1) ||
        (EEPROM_read(2) != STORAGE_SIG2))
    {
        EEPROM_update(3, 0);
    
        EEPROM_update(SECTOR_OFFSET, 1);
        for (uint8_t i = SECTOR_OFFSET + 1; i < SECTOR_OFFSET + SECTOR_SIZE; ++i)
        {
            EEPROM_update(i, 0);
        }
    
        EEPROM_update(0, STORAGE_SIG0);
        EEPROM_update(1, STORAGE_SIG1);
        EEPROM_update(2, STORAGE_SIG2);
    }
}

static int storage_get_free_sectors()
{
    int res = 0;
    for (uint8_t i = 0; i < SECTOR_SIZE; ++i)
    {
        uint8_t occupied = EEPROM_read(SECTOR_OFFSET + i);
        for (uint8_t j = 0x80 ; j ; j >>= 1)
        {
            if (!(occupied & j))
            {
                ++res;
            }
        }
    }
    return res;
}

static uint8_t storage_allocate_sector()
{
    for (uint8_t i = 0 ; i < SECTOR_SIZE ; ++i)
    {
        uint8_t occupied = EEPROM_read(SECTOR_OFFSET + i);
        for (uint8_t j = 0 ; j < 8 ; ++j)
        {
            if (!(occupied & (1 << j)))
            {
                uint8_t new_entry = i * 8 + j;
                // update sectors table
                EEPROM_update(i, occupied | (1 << j));
                return new_entry;
            }
        }
    }
    return 0;
}

static uint8_t storage_new_file(const char *name)
{
    uint8_t new_entry = storage_allocate_sector();
    if (new_entry)
    {
        // fill new entry
        int addr = SECTOR_OFFSET + SECTOR_SIZE * new_entry;
        for (; *name ; ++name, ++addr)
        {
            EEPROM_update(addr, *name);
        }
        EEPROM_update(addr, 0);
        EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * new_entry + FILENAME_LENGTH, 0);
        EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * new_entry + FILENAME_LENGTH + 1, 0);
        // find last directory entry
        uint8_t entry = storage_get_first_entry();
        uint8_t prev = 0;
        while (entry)
        {
            prev = entry;
            entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
        }
        if (prev == 0)
        {
            EEPROM_update(SECTOR_OFFSET - 1, new_entry);
        }
        else
        {
            EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * prev + FILENAME_LENGTH, new_entry); 
        }
        return new_entry;
    }
    return 0;
}

static uint8_t storage_find_file(const char *name)
{
    uint8_t entry = storage_get_first_entry();
    while (entry)
    {
        const char *p = name;
        bool found = true;
        for (uint8_t x = 0 ; found && x < FILENAME_LENGTH ; ++x, ++p)
        {
            char c = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + x);
            if (*p != c)
            {
                found = false;
            }
            else if (!*p)
            {
                break;
            }
        }
        if (found)
        {
            return entry;
        }
        entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
    }
    return 0;
}

static void storage_write_file(uint8_t entry, const void *buffer, size_t size)
{
    uint8_t prev = entry;
    entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
    while (size)
    {
        if (!entry)
        {
            // allocate new sector
            entry = storage_allocate_sector();
            EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * prev + DATA_SIZE, entry); 
        }
        uint8_t sz = size;
        if (sz > DATA_SIZE)
        {
            sz = DATA_SIZE;
        }
        EEPROM_update_buffer(SECTOR_OFFSET + SECTOR_SIZE * entry, buffer, sz);
        size -= sz;
        buffer += sz;
        prev = entry;
        entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + DATA_SIZE);
    }
}

static void storage_read_file(uint8_t entry, void *buffer, size_t size)
{
    entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + DATA_SIZE);
    while (size && entry)
    {
        uint8_t sz = size;
        if (sz > DATA_SIZE)
        {
            sz = DATA_SIZE;
        }
        EEPROM_read_buffer(SECTOR_OFFSET + SECTOR_SIZE * entry, buffer, sz);
        size -= sz;
        buffer += sz;
        entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + DATA_SIZE);
    }
}

bool storage_write(const char *name, const void *buffer, size_t size)
{
    uint8_t entry = storage_find_file(name);
    if (!entry)
    {
        entry = storage_new_file(name);
    }
    if (entry)
    {
        storage_write_file(entry, buffer, size);
    }
    /* TODO: check free space */
    return entry;
}

void storage_read(const char *name, void *buffer, size_t size)
{
    uint8_t entry = storage_find_file(name);
    if (entry)
    {
        /* TODO: check file size */
        storage_read_file(entry, buffer, size);
    }
}

#endif
