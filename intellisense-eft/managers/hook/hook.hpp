#pragma once

#include <dependencies/common_includes.hpp>

#include <game/structures/structures.hpp>

struct job_t;

namespace managers
{
    template<typename fn_def>
    class hook_t
    {
    public:
        fn_def original_fn;

        void hook_virtual_function( std::pair<std::uintptr_t, void*> addresses, std::pair<std::size_t, std::size_t> info )
        {
            const auto [object_address, function_address] = addresses;
            const auto [index, sz] = info;

            auto virtual_function_table = new void* [ sz ];

            std::memcpy( virtual_function_table, *reinterpret_cast< std::uintptr_t** >( object_address ), sz * sizeof( void* ) );

            original_fn = reinterpret_cast< fn_def >( virtual_function_table [ index ] );

            virtual_function_table [ index ] = function_address;

            *reinterpret_cast< std::uintptr_t* >( object_address ) = reinterpret_cast< std::uintptr_t >( virtual_function_table );
        }

        void hook( std::pair<std::uintptr_t, void*> addresses )
        {
            const auto [to_hook, function_address] = addresses;

            MH_CreateHook( reinterpret_cast< void* >( to_hook ), function_address, reinterpret_cast< void** >( &original_fn ) );
            MH_EnableHook( reinterpret_cast< void* >( to_hook ) );
        }

        template<typename... args_t>
        auto call_original( args_t... args ) -> std::invoke_result_t<fn_def, args_t...>
        {
            return original_fn( args... );
        }
    };

    class hook_manager_t
    {
    public:
        struct
        {
            hook_t < void*( __fastcall* )( void* rcx )> on_get_skinned_mesh_renderer;

            hook_t < void( __fastcall* )( void* game_world, void* player ) > on_register_player;
            hook_t < void( __fastcall* )( void* game_world, void* player ) > on_unregister_player;

            hook_t < void( __fastcall* )( void* game_world, void* grenade ) > on_register_grenade;
            hook_t < void( __fastcall* )( void* game_world, void* grenade ) > on_unregister_grenade;

            hook_t < void*(__fastcall*)(void* loot_item, void* item, void* itemName, void* gameWorld, bool randomRotation, void* validProfiles, void* staticId, bool performPickUpValidation) > on_loot_item_init;
            hook_t < void(__fastcall*)(void* loot_item, void* args) > on_remove_loot_item;

            hook_t < void( __fastcall* )( void* game_world ) > on_game_started;
            hook_t < void( __fastcall* )( void* game_world ) > on_game_world_dispose;

            hook_t < void( __fastcall* )(structures::exfiltration_point_t* extraction ) > on_exfil_awake;
            hook_t < void( __fastcall* )(structures::exfiltration_point_t* extraction ) > on_exfil_destroy;

            hook_t < void( __fastcall* )( void* game_world ) > on_game_world_update;

            hook_t < void( __fastcall* )( void* rcx ) > on_weapon_parameters_changed;

            hook_t < void* ( __fastcall* )( void* main_application ) > on_main_application_start;

            hook_t < void( __fastcall* )( void* rcx ) > on_sway_factor_update;

            hook_t < void( __fastcall* )( void* rcx, float dt ) > on_process_stamina;

            hook_t < void( __fastcall* )( void* rcx, float fov, float time, bool apply_camera ) > set_fov;
            hook_t < void( __fastcall* )( void* rcx, unsigned long value ) > set_time_date;

            hook_t < bool( __fastcall* )( void* rcx, int32_t c, bool val ) > set_physical_condition;
            hook_t < void( __fastcall* )( void* rcx, structures::player_t* plr, uint64_t collider ) > proceed_damage;

            hook_t < bool( __fastcall* )( void* rcx ) > suitable_hand_input;
            hook_t < float( __fastcall* )(void* rcx, void* ammoToFire, float overheat, double* durabilityMalfChance, float* magMalfChance, float* ammoMalfChance, float* overheatMalfChance, float* weaponDurability) > get_malfunction_chance;

            hook_t < float(__fastcall*)(void* rcx) > on_get_sprint_speed;

            hook_t < float(__fastcall*)(void* camera) > on_post_render;

            hook_t < void( __fastcall* )( void*, void*, std::int32_t, std::int32_t, structures::unity::vector3_t, structures::unity::vector3_t, float, float, float, float, float, float, float, float, float, float, std::int32_t, std::int32_t, void*, void*, float, void*, void*, std::int32_t, void* ) > on_create_shot;
            
            
            hook_t < void(__fastcall*)(void*, void*, void*) > test_hook;
        } hooks;

        void initiate( );

        void close( );
    };
}