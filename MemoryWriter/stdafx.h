// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include <windows.h>
#ifdef _DEBUG
#include <crtdbg.h>
//#include <debugapi.h>
#endif
#include <stdlib.h>
#include <sstream>
#include "LeakCheck.h"
#include "targetver.h"
#include "StringDefines.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>

#include <bemapiset.h>
#include <psapi.h>
#include <memory>

// TODO: reference additional headers your program requires here
#include "easylogging++.h"
