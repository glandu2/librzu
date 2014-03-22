#ifndef GUID_H
#define GUID_H

typedef struct {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} Guid;

#endif // GUID_H
