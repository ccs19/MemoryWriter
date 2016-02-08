#pragma once
#ifndef UNICODE  
typedef std::string String;
typedef std::ostringstream StringStream;
#else
typedef std::wstring String;
typedef std::wstringstream StringStream;
#endif
