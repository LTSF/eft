#pragma once

#include <dependencies/common_includes.hpp>

#include <game/structures/structures.hpp>

const auto text = ud::segment_t { ".text" };
const auto rdata = ud::segment_t { ".rdata" };

const auto mod = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( "UnityPlayer.dll" ) );

namespace managers
{
    class offset_manager_t
    {
    public:

        std::uintptr_t retcheck_gadget = [ ] ( ) -> std::uintptr_t
        {
            if ( const auto res = text.find_pattern< std::uintptr_t >( "ff e6" ) )
                return *res;

            return 0;
        }( );
    };
}

namespace offsets
{

}