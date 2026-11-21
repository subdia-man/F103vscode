#include "alglibmisc.h"
#include "ap.h"

class FileUIDGenerator {
public:

	FileUIDGenerator() {}
	~FileUIDGenerator() {}

	uint32_t GetNewUid() {
    	uint32_t randval = alglib_impl::ae_rand();
    	return randval;
	}
};