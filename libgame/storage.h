#ifndef STORAGE_H
#define STORAGE_H

#include <stdlib.h>

#define STORAGE_SIZE 1024
#define SECTOR_SIZE 12
#define SECTOR_OFFSET 4
#define SECTORS 85
#define FILENAME_LENGTH 10

#define STORAGE_CLOSED 0
#define STORAGE_READ 1
#define STORAGE_WRITE 2
#define STORAGE_FAILED 0xff
#define STORAGE_EOF 0xff

#define MAX_OPEN_FILES 4

#define STORAGE_SIG0 0xfc
#define STORAGE_SIG1 0x8b
#define STORAGE_SIG2 0x55

struct storage_file
{
    uint8_t prev_entry;
    uint8_t entry;
    uint8_t cur_sector;
    uint8_t cur_pos;
    uint8_t mode;
};

void storage_init();
void storage_format();

uint8_t storage_open(const char name[FILENAME_LENGTH], uint8_t mode);

void storage_write_byte(uint8_t sd, uint8_t value);
void storage_write_word(uint8_t sd, uint16_t value);
void storage_write_dword(uint8_t sd, uint32_t value);
size_t storage_write(uint8_t sd, void* buffer, size_t size); // returns amount of bytes written

uint8_t storage_read_byte(uint8_t sd);
uint16_t storage_read_word(uint8_t sd);
uint32_t storage_read_dword(uint8_t sd);
size_t storage_read(uint8_t sd, void* buffer, size_t size); // returns amount of bytes read

void storage_delete(uint8_t sd);
void storage_close(uint8_t sd);

#endif
