#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "12.12";

	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";

	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 4;
	static const long REVISION = 0;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4;
	#define RC_FILEVERSION 1,1,4,0
	#define RC_FILEVERSION_STRING "1, 1, 4, 0\0"
	static const char FULLVERSION_STRING[] = "1.1.4.0";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 4;


}
#endif //VERSION_H
