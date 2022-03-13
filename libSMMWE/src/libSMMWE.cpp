#include "libSMMWE.h"
#include <iostream>
#include <filesystem>
#include "Memory/Memory.h"
#include <Windows.h>
#include <atomic>

std::filesystem::path LocalAppData = mem::GetEnv("LOCALAPPDATA");
std::filesystem::path GamePath = LocalAppData / "SMM_WE";

void __cdecl SMMWE::hkdPersistentStep(void* _pSelf, void* _pOther)
{
	Sleep(20);
	if (running)
	{
		if (std::filesystem::exists(GamePath / "Textures"))
		{
			for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ GamePath / "Textures\\Default\\Sprites" })
			{
				if (dir_entry.is_regular_file() && dir_entry.path().extension().string().compare(".png") == 0)
				{
					std::string fname = dir_entry.path().filename().string();
					std::string Strip = fname;
					fname.erase(fname.find_last_of("_"));
					Strip.erase(0, Strip.find_last_of("_") + 6);
					Strip.erase(Strip.find_last_of("."));

					double sprite_index = GetSingleton().GetAssetIndex(_pSelf, _pOther, fname.c_str());

					if (sprite_index != -1)
					{
						GetSingleton().SpriteReplace(_pSelf, _pOther, sprite_index, dir_entry.path().string().c_str(), std::stoi(Strip), 0, 0, GetSingleton().GetSpriteXOrig(_pSelf, _pOther, sprite_index), GetSingleton().GetSpriteYOrig(_pSelf, _pOther, sprite_index));
					}
				}
			}

			for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ GamePath / "Textures\\Default\\Backgrounds" })
			{
				if (dir_entry.is_regular_file())
				{
					std::string fname = dir_entry.path().stem().string();

					double sprite_index = GetSingleton().GetAssetIndex(_pSelf, _pOther, fname.c_str());

					if (sprite_index != -1)
					{
						GetSingleton().BackgroundReplace(_pSelf, _pOther, sprite_index, dir_entry.path().string().c_str(), 0, 0);
					}
				}
			}
		}
		running = false;
	}
	oriPersistentStep(_pSelf, _pOther);
}

SMMWE& SMMWE::GetSingleton()
{
	static SMMWE instance;
	return instance;
}

void SMMWE::RegisterMethods(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4, uintptr_t arg5)
{
	oriPersistentStep = (gml_ObjectScript)arg1;
	YYGML_CallLegacyFunction = (_YYGML_CallLegacyFunction)arg2;
	Variable_GetValue_Direct = (_Variable_GetValue_Direct)arg3;
	AllocString = (_AllocString)arg4;
	DeAllocString = (_DeAllocString)arg5;
}

void SMMWE::Hook()
{
	oriPersistentStep = (gml_ObjectScript)mem::TrampHook32((BYTE*)oriPersistentStep, (BYTE*)hkdPersistentStep, 5);
}

void SMMWE::RejectHook()
{
	mem::DejectTrampHook32((BYTE*)oriPersistentStep, 5);
}

YYRValue SMMWE::SpriteReplace(void* _pSelf, void* _pOther, YYRValue ind, const char* fname, YYRValue imgnumb, YYRValue removeback, YYRValue smooth, YYRValue xorig, YYRValue yorig)
{
	YYRValue result;
	YYRValue string;
	YYRValue* args[] = { &ind, &string, &imgnumb, &removeback, &smooth, &xorig, &yorig };

	AllocString(&string, fname);
	YYGML_CallLegacyFunction(_pSelf, _pOther, &result, 7, 813, args);
	DeAllocString(&string);
	return result;
}

YYRValue SMMWE::BackgroundReplace(void* _pSelf, void* _pOther, YYRValue ind, const char* fname, YYRValue removeback, YYRValue smooth)
{
	YYRValue result;
	YYRValue string;
	YYRValue* args[] = { &ind, &string, &removeback, &smooth };

	AllocString(&string, fname);
	YYGML_CallLegacyFunction(_pSelf, _pOther, &result, 4, 837, args);
	DeAllocString(&string);
	return result;
}

YYRValue SMMWE::GetAssetIndex(void* _pSelf, void* _pOther, const char* name)
{
	YYRValue result;
	YYRValue ret;
	YYRValue string;
	YYRValue* args[] = { &string };

	AllocString(&string, name);
	YYGML_CallLegacyFunction(_pSelf, _pOther, &result, 1, 976, args);
	DeAllocString(&string);

	return result;
}

YYRValue SMMWE::GetSpriteXOrig(void* _pSelf, void* _pOther, YYRValue ind)
{
	if (ind == -1) return -1.0;

	YYRValue result;
	YYRValue* args[] = { &ind };

	YYGML_CallLegacyFunction(_pSelf, _pOther, &result, 1, 790, args);
	return result;
}

YYRValue SMMWE::GetSpriteYOrig(void* _pSelf, void* _pOther, YYRValue ind)
{
	if (ind == -1) return -1.0;

	YYRValue result;
	YYRValue* args[] = { &ind };

	YYGML_CallLegacyFunction(_pSelf, _pOther, &result, 1, 791, args);
	return result;
}