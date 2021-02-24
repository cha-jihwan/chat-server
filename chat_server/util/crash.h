#pragma once
#pragma warning(push)
#pragma warning(disable: 6011)

#define assert_if_false_inline( is_true ) \
if ( is_true == false )		\
{							\
	int* hazard_ptr{};		\
	*hazard_ptr = 0xDEAD;	\
}							\


#define crash_inline()			\
{								\
	hazard_ptr{};		\
	*hazard_ptr = 0xDEAD;		\
}					  


void crash_if_false(bool is_true);
void crash();
void break_point();

#pragma warning(pop)