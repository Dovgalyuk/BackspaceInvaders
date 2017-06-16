#ifndef EMULATED

#include "storage.h"
#include <avr/eeprom.h>
#include <string.h>

uint8_t EEPROM_read(int addr)
{
    return eeprom_read_byte((uint8_t*)addr);
}

void EEPROM_write(int addr, uint8_t value)
{
    eeprom_write_byte((uint8_t*)addr, value);
}

void EEPROM_update(int addr, uint8_t value)
{
    eeprom_update_byte((uint8_t*)addr, value);
}

storage_file open_files[MAX_OPEN_FILES];
uint8_t occupied_sectors[SECTOR_SIZE];
uint8_t first_entry;

bool sector_is_free(uint8_t sector)
{
    return !((EEPROM_read(SECTOR_OFFSET + (sector >> 3)) >> (sector & 7)) & 1);
}

void storage_init()
{
    if ((EEPROM_read(0) != STORAGE_SIG0) ||
        (EEPROM_read(1) != STORAGE_SIG1) ||
        (EEPROM_read(2) != STORAGE_SIG2)) storage_format();
    first_entry = EEPROM_read(3);
    for (uint8_t i = 0; i < SECTOR_SIZE; ++i)
    {
        occupied_sectors[i] = EEPROM_read(SECTOR_OFFSET + i);
    }
    for (uint8_t i = 0; i < MAX_OPEN_FILES; ++i)
    {
        open_files[i].mode = STORAGE_CLOSED;
    }
}

void storage_flush_sectors()
{
    for (uint8_t i = 0; i < SECTOR_SIZE; ++i)
    {
        EEPROM_update(SECTOR_OFFSET + i, occupied_sectors[i]);
    }
}

void storage_format()
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

uint8_t storage_open(const char name[FILENAME_LENGTH], uint8_t mode)
{
    uint8_t entry = first_entry;
    uint8_t prev_entry = 0;
    while (entry)
    {
        char entry_name[FILENAME_LENGTH];
        for (uint8_t x = 0; x < FILENAME_LENGTH; ++x)
            entry_name[x] = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + x);
        if (!strncmp(name, entry_name, FILENAME_LENGTH))
        {
            // this is our entry
            for (uint8_t i = 0; i < MAX_OPEN_FILES; ++i)
            {
                if (open_files[i].mode == STORAGE_CLOSED)
                {
                    open_files[i].prev_entry = prev_entry;
                    open_files[i].entry = entry;
                    open_files[i].cur_pos = 0;
                    open_files[i].cur_sector = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
                    open_files[i].mode = mode;

                    if (mode == STORAGE_READ)
                    {
                        return i;
                    }

                    if (mode == STORAGE_WRITE)
                    {
                        uint8_t cur_data = open_files[i].cur_sector;
                        while (cur_data)
                        {
                            // mark sector as empty
                            occupied_sectors[cur_data >> 3] &= ~(1 << (cur_data & 7));
                            cur_data = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * cur_data + SECTOR_SIZE - 1);
                        }
                        storage_flush_sectors();
                        return i;
                    }
                }
            }
            return STORAGE_FAILED;
        }
        prev_entry = entry;
        entry = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH + 1);
    }
    if (mode == STORAGE_WRITE)
    {
        for (uint8_t i = 0; i < MAX_OPEN_FILES; ++i)
        {
            if (open_files[i].mode == STORAGE_CLOSED)
            {
                open_files[i].prev_entry = prev_entry;
                open_files[i].cur_pos = 0;
                open_files[i].cur_sector = 0;
                open_files[i].mode = STORAGE_WRITE;

                // now we need to find an empty sector for entry
                for (uint8_t sector = 0; sector < SECTORS; ++sector)
                {
                    if (sector_is_free(sector))
                    {
                        open_files[i].entry = sector;
                        occupied_sectors[sector >> 3] |= 1 << (sector & 7);
                        for (uint8_t x = 0; x < FILENAME_LENGTH; ++x)
                            EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * sector + x, name[x]);
                        EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * sector + FILENAME_LENGTH, 0);  // data
                        EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * sector + FILENAME_LENGTH + 1, 0);  // next
                        if (prev_entry == 0)
                            EEPROM_update(SECTOR_OFFSET - 1, sector); // prev->next 
                        else
                            EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * prev_entry + FILENAME_LENGTH + 1, sector); // prev->next 
                        storage_flush_sectors();
                        return i;
                    }
                }

                return STORAGE_FAILED;
            }
        }
    }
    return STORAGE_FAILED;
}

size_t storage_read(uint8_t sd, void* _buffer, size_t size)
{
    if ((open_files[sd].mode != STORAGE_READ) || (open_files[sd].cur_pos == STORAGE_EOF))
    {
        return 0;
    }
    uint8_t* buffer = (uint8_t*)_buffer;
    size_t r = 0;
    while (r < size)
    {
        *buffer = EEPROM_read(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + open_files[sd].cur_pos);
        buffer++;
        r++;
        open_files[sd].cur_pos++;
        if (open_files[sd].cur_pos >= SECTOR_SIZE - 1)
        {
            // next sector
            uint8_t ns = EEPROM_read(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + SECTOR_SIZE - 1);
            if (ns == 0)
            {
                open_files[sd].cur_pos = STORAGE_EOF;
                break;
            }
            else
            {
                open_files[sd].cur_pos = 0;
                open_files[sd].cur_sector = ns;
            }
        }
    }
    return r;
}

uint8_t storage_read_byte(uint8_t sd)
{
    uint8_t r = 0;
    storage_read(sd, (void*)&r, sizeof(r));
    return r;
}

uint16_t storage_read_word(uint8_t sd)
{
    uint16_t r = 0;
    storage_read(sd, (void*)&r, sizeof(r));
    return r;
}

uint32_t storage_read_dword(uint8_t sd)
{
    uint32_t r = 0;
    storage_read(sd, (void*)&r, sizeof(r));
    return r;
}

size_t storage_write(uint8_t sd, void* _buffer, size_t size)
{
    if (open_files[sd].mode != STORAGE_WRITE)
    {
        return 0;
    }
    uint8_t* buffer = (uint8_t*)_buffer;
    size_t w = 0;
    while (w < size)
    {
        if ((open_files[sd].cur_pos >= SECTOR_SIZE - 1) || (open_files[sd].cur_sector == 0))
        {
            // allocate next sector
            bool allocated = false;
            for (uint8_t sector = 0; sector < SECTORS; ++sector)
            {
                if (sector_is_free(sector))
                {
                    allocated = true;
                    if (open_files[sd].cur_sector == 0)
                        EEPROM_update(SECTOR_OFFSET + open_files[sd].entry * SECTOR_SIZE + FILENAME_LENGTH, sector);
                    else
                        EEPROM_update(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + SECTOR_SIZE - 1, sector);
                    open_files[sd].cur_pos = 0;
                    open_files[sd].cur_sector = sector;
                    occupied_sectors[sector >> 3] |= 1 << (sector & 7);
                    storage_flush_sectors();
                    break;
                }
            }
            if (!allocated) break;
        }
        EEPROM_update(SECTOR_OFFSET + open_files[sd].cur_sector * SECTOR_SIZE + open_files[sd].cur_pos, *buffer);
        buffer++;
        w++;
        open_files[sd].cur_pos++;
    }
    return w;
}

void storage_write_byte(uint8_t sd, uint8_t value)
{
    storage_write(sd, (void*)&value, sizeof(value));
}

void storage_write_word(uint8_t sd, uint16_t value)
{
    storage_write(sd, (void*)&value, sizeof(value));
}

void storage_write_dword(uint8_t sd, uint32_t value)
{
    storage_write(sd, (void*)&value, sizeof(value));
}

void storage_close(uint8_t sd)
{
    open_files[sd].mode = STORAGE_CLOSED;
}

void storage_delete(uint8_t sd)
{
    uint8_t next = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * open_files[sd].entry + FILENAME_LENGTH + 1);
    if (open_files[sd].prev_entry == 0)
        EEPROM_update(SECTOR_OFFSET - 1, next); 
    else
        EEPROM_update(SECTOR_OFFSET + SECTOR_SIZE * open_files[sd].prev_entry + FILENAME_LENGTH + 1, next);
    open_files[sd].mode = STORAGE_CLOSED;
    uint8_t entry = open_files[sd].entry;
    uint8_t cur_data = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * entry + FILENAME_LENGTH);
    occupied_sectors[entry >> 3] &= ~(1 << (entry & 7));
    while (cur_data)
    {
        // mark sector as empty
        occupied_sectors[cur_data >> 3] &= ~(1 << (cur_data & 7));
        cur_data = EEPROM_read(SECTOR_OFFSET + SECTOR_SIZE * cur_data + SECTOR_SIZE - 1);
    }
    storage_flush_sectors();
}

#endif
