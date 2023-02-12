#include <dependencies/common_includes.hpp>

#include <managers/managers.hpp>

constexpr auto bypass = false;

using namespace managers;

void entry( )
{
    utilities::io::initiate( "eft" );

    utilities::io::log( "Injected\n" );

    mgrs.interface_mgr.initiate( );
    mgrs.hook_mgr.initiate();

    mono::functions::thread_attach(mono::functions::get_root_domain());

    const auto main_application_start = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
        ->get_class( "EFT", "MainApplication" )->get_method_from_name( "Init", -1 )
            ->get_function_pointer< std::uint8_t* >( );

    mgrs.hook_mgr.hooks.on_main_application_start.hook(
        { reinterpret_cast< std::uintptr_t >( main_application_start ),
            [ ] ( void* main_application ) -> void*
            {
                const auto res = mgrs.hook_mgr.hooks.on_main_application_start.call_original( main_application );

                utilities::io::log( "MainApplication called : %p\n", main_application );

                mgrs.feature_mgr.initiate( );

                return res;
            }
        }
    );

    utilities::io::log( "Initialized\n" );
}

bool DllMain( HMODULE instance, std::uint32_t reason, void* )
{
    if constexpr ( bypass )
    {
        if ( reason == DLL_PROCESS_ATTACH )
        {
            wchar_t file_name [ MAX_PATH ];
            GetModuleFileNameW( instance, file_name, MAX_PATH );
            LoadLibraryW( file_name );
        }
    }
    else
    {
        if ( reason == DLL_PROCESS_ATTACH )
            std::thread { entry }.detach( );
    }

    return true;
}

std::atomic_bool initialized = false;
extern "C" __declspec( dllexport ) LRESULT hook( std::int32_t code, WPARAM wparam, LPARAM lparam )
{
    if ( !initialized.exchange( true ) )
    {
        std::thread { entry }.detach( );
    }

    return CallNextHookEx( nullptr, code, wparam, lparam );
}