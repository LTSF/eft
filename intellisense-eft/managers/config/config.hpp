#pragma once

#include <dependencies/common_includes.hpp>

#include <variant>

enum features_t
{
    infinite_stamina,
    run_heal,
    remove_barbed,
    owl_look,
    loot_distance,
    loot_through_walls,

    fov_offset,
    no_visor,
    aspect_ratio,
    time_changer_enable,
    time_changer_hour,

    always_shoot,
    anti_malfunction,
    no_recoil,
    no_sway,

    silent_aim,
    silent_aim_linecast,
    silent_aim_lock_target,
    silent_aim_fov,

    esp_type,
    chams_esp,
    name_esp,
    weapon_esp,
    distance_esp,
    health_bar_esp,

    loot_esp,
    loot_esp_min_value,

    feature_count
};

enum esp_type_t : int
{
    none = 0,
    filled,
    bounding,
    corner
};

namespace managers
{
    class config_manager_t
    {
        class feature_t
        {
            std::variant< bool, float, int, struct ImColor, esp_type_t > internal_value;

        public:

            std::string serialize_feature();

            template< typename t >
            feature_t(t value)
                : internal_value{ value } { }

            template< typename t >
            t& get_value()
            {
                return std::get< t >(internal_value);
            }
        };

        std::array< feature_t, features_t::feature_count > feature_array =
        {
            feature_t{ true }, /*infinite_stamina*/
            feature_t{ true }, /*run_heal*/
            feature_t{ true }, /*remove_barbed*/
            feature_t{ true }, /*owl_look*/
            feature_t{ 4.f }, /*loot_distance*/
            feature_t{ true }, /*loot_through_walls*/

            feature_t{ 10.f }, /*fov_offset*/
            feature_t{ false }, /*no_visor*/
            feature_t{ 75 }, /*aspect_ratio*/
            feature_t{ true }, /*time_changer_enable*/
            feature_t{ 12.f }, /*time_changer_hour*/

            feature_t{ true }, /*always_shoot*/
            feature_t{ true }, /*anti_malfunction*/
            feature_t{ true }, /*no_recoil*/
            feature_t{ true }, /*no_sway*/

            feature_t{ true }, /*silent_aim*/
            feature_t{ false }, /*silent_aim_linecast*/
            feature_t{ true }, /*silent_aim_lock_target*/
            feature_t{ 25.f }, /*silent_aim_fov*/

            feature_t{ 2 }, /*esp_type*/
            feature_t{ true }, /*chams_esp*/
            feature_t{ true }, /*name_esp*/
            feature_t{ true }, /*weapon_esp*/
            feature_t{ true }, /*distance_esp*/
            feature_t{ true }, /*health_bar_esp*/

            feature_t{ true }, /*loot_esp*/
            feature_t{ 50000 }, /*loot_esp_min_value*/
        };

    public:
        template< typename t >
        t& get(features_t feature)
        {
            return feature_array[feature].get_value< t >();
        }
    };
}