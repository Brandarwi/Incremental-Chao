// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include "..\SA2programming\ninja.h"
//#include "..\SA2programming\njdef.h"//ninja.h //WEIRD FILE DOES NOT COMPILE

//#include "..\SA2programming\MemAccess.h"
//#include "..\SA2programming\FunctionHook.h"//MemAccess.h
//#include "..\SA2programming\UsercallFunctionHandler.h"//MemAccess.h
//#include "..\SA2programming\magic.h"//MemAccess.h, SA2Structs.h

#include "..\SA2programming\SA2Enums.h"//ninja.h
#include "..\SA2programming\SA2Structs.h"//ninja.h, SA2Enums.h
#include "..\SA2programming\SA2ModInfo.h"//SA2Structs.h
#include "..\SA2programming\SA2Variables.h"//MemAccess.h, SA2Structs.h
#include "..\SA2programming\SA2Functions.h"//ninja.h, MemAccess.h, SA2Enums.h, SA2Structs.h
#include "..\SA2programming\SA2ModLoader.h"// Brandarwi ninja.h.  (old)MemAccess.h, SA2Enums.h, SA2Structs.h, SA2ModInfo.h, SA2Variables.h, SA2Functions.h

#include "..\UserHeaders\shadd.h"

#endif //PCH_H
