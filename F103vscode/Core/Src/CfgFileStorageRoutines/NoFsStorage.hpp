#ifndef NO_FS_STORAGE_H
#define NO_FS_STORAGE_H

#define MAX_FILES_NUMBER 300

typedef struct {
	uint16_t header;
	uint16_t filesNumber;
	uint32_t fAddresses[MAX_FILES_NUMBER];
	uint32_t fUIDs[MAX_FILES_NUMBER];
} StorageStruct_t;

#endif