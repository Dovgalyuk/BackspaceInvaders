#ifndef STORAGE_H
#define STORAGE_H

#include <stdlib.h>
#include <stdint.h>

void storage_init();
void storage_format();

void storage_read(const char *name, void *buffer, size_t size);
bool storage_write(const char *name, const void *buffer, size_t size);

#endif
