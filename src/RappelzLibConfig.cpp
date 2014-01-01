#include "RappelzLibConfig.h"

RappelzLibConfig* RappelzLibConfig::get() {
	static RappelzLibConfig instance;

	return &instance;
}
