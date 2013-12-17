#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "CircularBuffer.h"
#include "uv.h"

CircularBuffer::CircularBuffer(size_t size) {
	buffer = NULL;
	dataSize = 0;
	addCount = 0;
	dataIn = dataOut = NULL;
	printBufferPos = 0;
	uv_mutex_init(&lock);

	if(size > 0)
		setBufferSize(size);
}

CircularBuffer::~CircularBuffer() {
	if(buffer) free(buffer);
}


size_t CircularBuffer::insertData(char *data, size_t size) {
	size_t effectiveSize;
	uv_mutex_lock(&lock);

	if(size > dataSize)
		size = dataSize;


	if(addCount + size > dataSize) {
		size = dataSize - addCount;
	}

	effectiveSize = size;
	addCount += size;

	if(buffer - dataIn + dataSize < size) {	//si on va dépasser la taille du buffer
		memcpy(dataIn, data, buffer - dataIn + dataSize);
		size -= (int)(buffer - dataIn) + dataSize;
		data += buffer - dataIn + dataSize;
		dataIn = buffer;
	}
	memcpy(dataIn, data, size);
	dataIn += size;

	uv_mutex_unlock(&lock);

	return effectiveSize;
}

size_t CircularBuffer::getData(char *data, size_t size) {
	size_t effectiveSize;
	uv_mutex_lock(&lock);

	if(size > dataSize)
		size = dataSize;

	if(addCount < size)
		size = addCount;

	effectiveSize = size;
	addCount -= size;

	if(buffer - dataOut + dataSize < size) {	//si on va dépasser la taille du buffer
		memcpy(data, dataOut, buffer - dataOut + dataSize);
		size -= (int)(buffer - dataOut) + dataSize;
		data += buffer - dataOut + dataSize;
		dataOut = buffer;
	}
	memcpy(data, dataOut, size);
	dataOut += size;

	uv_mutex_unlock(&lock);
	#ifdef CIRCULARBUFFER_VIEWSTATUS
		printBuffersPos();
	#endif

	return effectiveSize;
}

void CircularBuffer::setBufferSize(size_t size) {
	uv_mutex_lock(&lock);

	if(buffer != NULL) free(buffer);

	if(size > 0) {
		buffer = (char*)malloc(size);
		if(!buffer) fprintf(stderr, "WARNING NO MEMORY");

		dataSize = size;
		dataOut = buffer;
		dataIn = buffer; //+ dataSize/2;
	} else {
		buffer = NULL;
		dataSize = 0;
		dataOut = NULL;
		dataIn = NULL;
	}

	addCount = 0;

	uv_mutex_unlock(&lock);
}

#define PRTDBG_WDT 65

void CircularBuffer::printBuffersPos() {
	char out[PRTDBG_WDT];

	printBufferPos = !printBufferPos;
	if(printBufferPos) return;

	memset(out, '-', PRTDBG_WDT-1);
	out[PRTDBG_WDT-1] = 0;

	uv_mutex_lock(&lock);

	out[(dataIn-buffer)*(PRTDBG_WDT-2)/dataSize] = ']';
	out[(dataOut-buffer)*(PRTDBG_WDT-2)/dataSize] = '[';
	printf("%6ld,%6ld}", dataIn-buffer, dataOut-buffer);

	uv_mutex_unlock(&lock);

	puts(out);
}

void CircularBuffer::getMaxContinuousBuffer(char** data, size_t* len) {
	size_t size = *len;
	uv_mutex_lock(&lock);

	if(size > dataSize)
		size = dataSize;

	if(addCount + size > dataSize) {
		size = dataSize - addCount;
	}

	if(buffer - dataIn + dataSize < size) {	//si on va dépasser la taille du buffer
		*data = dataIn;
		*len = buffer - dataIn + dataSize;

		addCount += *len;
		dataIn = buffer;
	} else {
		*data = dataIn;
		*len = size;

		addCount += *len;
		dataIn += size;
	}

	uv_mutex_unlock(&lock);
}

size_t CircularBuffer::getAvailableBytes() {
	size_t count;

	uv_mutex_lock(&lock);
	count = addCount;
	uv_mutex_unlock(&lock);

	return count;
}
