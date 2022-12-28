#ifndef SA2MODLOADER_H
#define SA2MODLOADER_H

#if !defined(_M_IX86) && !defined(__i386__)
#error Mods must be built targeting 32-bit x86, change your settings.
#endif

#include <WTypes.h>
//Brandarwi: a file should include headers of code it uses.  Got rid of "broad" headers that were only used to reduce size of pch.h
//#include <cstdint>
#include "ninja.h"//The only include code used in this file and it was assumed from the other includes :(
//#include "MemAccess.h"
//#include "SA2ModInfo.h"
//#include "SA2Enums.h"
//#include "SA2Structs.h"
//#include "SA2Variables.h"
//#include "SA2Functions.h"
//Brandarwi

static inline void ResizeTextureList(NJS_TEXLIST* texlist, Uint32 count)
{
	texlist->textures = new NJS_TEXNAME[count]{};
	texlist->nbTexture = count;
}

static inline void ResizeTextureList(NJS_TEXLIST* texlist, NJS_TEXNAME* textures, Uint32 count)
{
	texlist->textures = textures;
	texlist->nbTexture = count;
}

template <Uint32 N>
static inline void ResizeTextureList(NJS_TEXLIST* texlist, NJS_TEXNAME(&textures)[N])
{
	ResizeTextureList(texlist, textures, N);
}
#endif