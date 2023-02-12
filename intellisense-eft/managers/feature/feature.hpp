#pragma once

#include <dependencies/common_includes.hpp>

#include <game/structures/structures.hpp>

#include <functional>

namespace managers
{
    class feature_manager_t
    {
    public:
        struct
        {
            structures::unity::shader_t* cham_shader;
        } shaders;

        class 
        {
            std::mutex players_locker, loot_locker, grenades_locker, camera_locker, game_world_locker, exfils_locker, update_queue_locker, shader_cache_locker;

            structures::game_world_t* game_world;

            structures::unity::camera_t* camera;
            structures::unity::vector3_t camera_pos;

            structures::player_t* local_player;

            structures::player_t* closest_player;

            std::vector< structures::player_t* > players;

            std::vector< structures::loot_entry_t > loot;

            std::vector< structures::throwable_t* > grenades;

            std::vector< structures::exfiltration_point_t* > exfils;

            std::vector< std::function< void() > > update_queue;

            std::unordered_map< structures::player_t*, structures::unity::shader_t* > shader_cache;
        public:

            structures::unity::shader_t* get_old_shader(structures::player_t* player)
            {
                std::unique_lock locker{ shader_cache_locker };

                if(shader_cache.contains(player))
                    return shader_cache.at(player);

                return nullptr;
            }

            void cache_shader(structures::player_t* player, structures::unity::shader_t* shader)
            {
                std::unique_lock locker{ shader_cache_locker };
                shader_cache.insert({ player, shader });
            }

            void clear_shaders()
            {
                std::unique_lock locker{ shader_cache_locker };
                shader_cache.clear();
            }

            void add_to_update_queue(std::function< void() > callback)
            {
                std::unique_lock locker{ update_queue_locker };
                update_queue.push_back(callback);
            }

            std::vector< std::function< void() > > get_update_queue()
            {
                std::unique_lock locker{ update_queue_locker };
                return update_queue;
            }

            void clear_update_queue()
            {
                std::unique_lock locker{ update_queue_locker };
                update_queue.clear();
            }

            structures::game_world_t* get_game_world()
            {
                std::unique_lock locker{ game_world_locker };
                return game_world;
            }

            void set_game_world(structures::game_world_t* gw)
            {
                std::unique_lock locker{ game_world_locker };
                game_world = gw;
            }

            structures::unity::camera_t* get_camera()
            {
                std::unique_lock locker{ camera_locker };
                return camera;
            }

            void set_camera(structures::unity::camera_t* cam)
            {
                std::unique_lock locker{ camera_locker };
                camera = cam;
            }

            void set_camera_pos(structures::unity::vector3_t pos) {
                std::unique_lock locker{ camera_locker };
                camera_pos = pos;
            }

            structures::unity::vector3_t get_camera_pos() {
                std::unique_lock locker{ camera_locker };
                return camera_pos;
            }

            void set_closest_player( structures::player_t* closest_plr )
            {
                std::unique_lock locker { players_locker };
                closest_player = closest_plr;
            }

            structures::player_t* get_closest_player( )
            {
                std::unique_lock locker { players_locker };
                return closest_player;
            }

            void push_grenade( structures::throwable_t* grenade )
            {
                std::unique_lock locker { grenades_locker };
                grenades.push_back( grenade );
            }

            void destroy_grenade( structures::throwable_t* grenade )
            {
                std::unique_lock locker { grenades_locker };
                if ( const auto iter = std::find( grenades.begin( ), grenades.end( ), grenade ); iter != grenades.end( ) )
                    grenades.erase( iter );
            }

            std::vector< structures::throwable_t* > get_grenades( )
            {
                std::unique_lock locker { grenades_locker };

                return grenades;
            }

            void push_loot(structures::loot_entry_t loot_item)
            {
                std::unique_lock locker { loot_locker };
                loot.push_back(loot_item);
            }

            void destroy_loot(structures::loot_item_t* loot_item )
            {
                std::unique_lock locker { loot_locker };

                if (const auto iter = std::find_if(loot.begin(), loot.end(), [loot_item](structures::loot_entry_t entr) {return entr.loot_item == loot_item; }); iter != loot.end( ) )
                    loot.erase( iter );
            }

            std::vector< structures::loot_entry_t > get_loot( )
            {
                std::unique_lock locker { loot_locker };

                return loot;
            }

            void push_exfil( structures::exfiltration_point_t* exfil )
            {
                std::unique_lock locker { exfils_locker };
                exfils.push_back(exfil);
            }

            void destroy_exfil( structures::exfiltration_point_t* exfil)
            {
                std::unique_lock locker { exfils_locker };
                if ( const auto iter = std::find( exfils.begin( ), exfils.end( ), exfil); iter != exfils.end( ) )
                    exfils.erase( iter );
            }

            std::vector< structures::exfiltration_point_t* > get_exfils( )
            {
                std::unique_lock locker { exfils_locker };

                return exfils;
            }

            void add_player(structures::player_t* player)
            {
                std::unique_lock locker{ players_locker };
                players.push_back(player);
            }

            void remove_player(structures::player_t* player)
            {
                std::unique_lock locker{ players_locker };

                if (const auto iter = std::find(players.begin(), players.end(), player); iter != players.end())
                    players.erase(iter);
            }

            void clear_grenades()
            {
                std::unique_lock locker{ grenades_locker };
                grenades.clear();
            }

            void clear_loot()
            {
                std::unique_lock locker{ loot_locker };
                loot.clear();
            }

            void clear_exfils()
            {
                std::unique_lock locker{ exfils_locker };
                exfils.clear();
            }

            void clear_players()
            {
                std::unique_lock locker{ players_locker };
                players.clear();
            }

            std::vector< structures::player_t* > get_players( )
            {
                std::unique_lock locker { players_locker };

                return players;
            }

            structures::player_t* get_local_player( )
            {
                std::unique_lock locker { players_locker };
                return local_player;
            }

            void set_local_player(structures::player_t* player)
            {
                std::unique_lock locker{ players_locker };
                local_player = player;
            }
        } globals;

        void setup_callback( );
        void free_callback( );

        std::atomic_bool in_game;

        void initiate( );

        void visual_callback( );
    };
}