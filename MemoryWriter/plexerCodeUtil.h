#pragma once


namespace plexerCode {
	class util {
	public:
		/**
		 Allocates memory for DWORD array and initializes to zero.
		*/
		static DWORD* initDWord(unsigned long size) {
			auto* dword = new DWORD[size];
			memset(dword, 0, size);
			return dword;
		}

		

	private:
		
	};

};
