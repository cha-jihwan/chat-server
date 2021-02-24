#include "../common/pre_compile.h"

void crash_if_false(bool is_true)
{
	if (is_true == false)
	{
		int* hazard_ptr{};
		*hazard_ptr = 0xDEAD;
	}
}

void crash()
{
	size_t* hazard_ptr{};
#if _M_AMD64
	* hazard_ptr = 0xDDEEAADDBBEEEEFF;
#else  
	* hazard_ptr = 0xDEAD;
#endif
}


void break_point()
{
	//#ifdef _DEBUG
#if _M_AMD64
	__debugbreak(); // AMD64
#else  
	__asm int 3; // WIN32
#endif
//#endif // _DEBUG
}