#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include "Object.h"
#include "uv.h"

//#define CIRCULARBUFFER_VIEWSTATUS

class RAPPELZLIB_EXTERN CircularBuffer : public Object
{
	DECLARE_CLASSNAME(CircularBuffer, 0)

	protected:
		char *buffer;
		char *dataIn, *dataOut;
		size_t dataSize, addCount;
		uv_mutex_t lock;
		int printBufferPos;

	public:
		CircularBuffer(size_t size = 0);
		~CircularBuffer();

		size_t insertData(char *data, size_t size);
		size_t getData(char *data, size_t size);
		void setBufferSize(size_t size);
		void printBuffersPos();
		void getMaxContinuousBuffer(char **buffer, size_t* len);
		size_t getAvailableBytes();
};

#endif
