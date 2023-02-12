#include "hook.hpp"

void managers::hook_manager_t::initiate( )
{
	MH_Initialize( );
}

void managers::hook_manager_t::close( )
{
	MH_Uninitialize( );
}