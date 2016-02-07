#pragma once
#include <bemapiset.h>

namespace plexerCode {
	/**
	 Allocates memory for DWORD array and initializes to zero.
	*/
	static DWORD* initDWord(unsigned long size) {
		auto* dword = new DWORD[size];
		memset(dword, 0, size);
		return dword;
	}
}
