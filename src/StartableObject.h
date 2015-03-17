#ifndef ISTARTABLESESSION_H
#define ISTARTABLESESSION_H

#include <stdint.h>
#include <string>
#include "IListener.h"

class RZU_EXTERN StartableObject : public IListener
{
public:
	StartableObject();
	~StartableObject();

	virtual bool start() = 0;
	virtual void stop() = 0;
	virtual bool isStarted() = 0;

	const char* getName() { return name ? name : ""; }
	void setName(const char* name);

private:
	char* name;
};

#endif // ISTARTABLESESSION_H
