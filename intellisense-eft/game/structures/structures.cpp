#include "structures.hpp"

#include <managers/managers.hpp>
#include "../../game/item_list/items.hpp"
#include <utilities/utilities.hpp>

using namespace managers;

structures::unity::material_t* structures::unity::renderer_t::get_material() const
{
	static const auto renderer_get_material = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Renderer")->get_method_from_name("GetMaterial", -1)
		->get_function_pointer< structures::unity::material_t* (__fastcall*)(const structures::unity::renderer_t*) >();

	return renderer_get_material(this);
}

void structures::unity::material_t::set_shader(void* shader) const
{
	static const auto material_set_shader = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Material")->get_method_from_name("set_shader", -1)
		->get_function_pointer< structures::unity::material_t* (__fastcall*)(const material_t*, void*) >();

	material_set_shader(this, shader);
}

structures::unity::shader_t* structures::unity::material_t::get_shader() const
{
	static const auto material_get_shader = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Material")->get_method_from_name("get_shader", -1)
		->get_function_pointer< structures::unity::shader_t* (__fastcall*)(const material_t*) >();

	return material_get_shader(this);
}

void structures::unity::material_t::set_float(const str_t& name, float f) const
{
	//static const auto material_set_float = mono::structures::image_t::get_image_by_name( "UnityEngine.CoreModule" )
	//	->get_class( "UnityEngine", "Material" )->get_method_from_name( "SetInt", -1 )
	//		->get_function_pointer< structures::unity::material_t* ( __fastcall* )( const material_t*, void* ) >( );

	//material_set_float( this, shader );

	//mgrs.offset_mgr.set_material_float( this, mgrs.offset_mgr.property_to_id( name ), f );
}

void structures::unity::material_t::set_int(const str_t& name, int i) const
{
	static const auto material_set_int = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Material")->get_method_from_name("SetInt", -1)
		->get_function_pointer< structures::unity::material_t* (__fastcall*)(const material_t*, str_t, int) >();

	material_set_int(this, name, i);
}

void structures::unity::material_t::set_color(const str_t& name, const color_t& color) const
{
	static const auto material_set_color = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Material")->get_method_from_name("SetColor", -1)
		->get_function_pointer< structures::unity::material_t* (__fastcall*)(const material_t*, str_t, color_t) >();

	material_set_color(this, name, color);
}

structures::unity::list_t< structures::player_t* >* structures::game_world_t::get_players() const
{
	if (!this)
		return nullptr;

	static const auto players = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "GameWorld")->get_field("RegisteredPlayers")->get_offset();

	return *reinterpret_cast<unity::list_t< player_t* >**>(reinterpret_cast<std::uintptr_t>(this) + players);
}

structures::unity::list_t< structures::loot_t* >* structures::game_world_t::get_loot() const
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<unity::list_t< loot_t* >**>(reinterpret_cast<std::uintptr_t>(this) + 0x68);
}

structures::unity::list_t< structures::throwable_t* >* structures::game_world_t::get_grenades() const
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<unity::list_t< throwable_t* >**>(reinterpret_cast<std::uintptr_t>(this) + 0xF8);
}

structures::game_world_t* structures::game_world_t::get()
{
	const auto game_world = unity::base_object_t::find(L"GameWorld");

	if (game_world && game_world->game_object)
	{
		if (const auto class_object = game_world->game_object->class_object)
			return *reinterpret_cast<structures::game_world_t**>(class_object->object_reference + 0x28);
	}

	return nullptr;
}

structures::unity::game_object_t* structures::unity::base_object_t::get_game_object() const
{
	static const auto get_game_object = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Component")->get_method_from_name("get_gameObject", -1)
		->get_function_pointer< structures::unity::game_object_t* (__fastcall*)(void*) >();

	return get_game_object((void*)this);
}

structures::unity::base_object_t* structures::unity::base_object_t::find(const str_t& name)
{
	static const auto find_game_object = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "GameObject")->get_method_from_name("Find", -1)
		->get_function_pointer< structures::unity::base_object_t* (__fastcall*)(str_t) >();

	return find_game_object(name);
}

void structures::shot_effector_t::set_recoil_strength(float value) const
{
	static const auto strength_xy = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "ShotEffector")->get_field("RecoilStrengthXy")->get_offset();

	static const auto strength_z = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "ShotEffector")->get_field("RecoilStrengthZ")->get_offset();

	if (!this)
		return;

	const auto address = reinterpret_cast<std::uintptr_t>(this);

	for (auto i = address + strength_xy; i < address + strength_z; ++i)
		*reinterpret_cast<float*>(i) = 0.0f;
}

structures::inventory_controller_t* structures::player_t::get_inventory_controller() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("_inventoryController")->get_offset();

	return *reinterpret_cast<structures::inventory_controller_t**>((std::uintptr_t)this + offset);
}

structures::player_body_t* structures::player_t::get_player_body() const
{
	static const auto player_body = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_field("_playerBody")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<player_body_t**>(reinterpret_cast<std::uintptr_t>(this) + player_body);
}

structures::unity::vector2_t structures::player_t::get_rotation() const
{
	static const auto get_rotation = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_method_from_name("get_Rotation", -1)
		->get_function_pointer< structures::unity::vector2_t(__fastcall*)(const void* player) >();

	if (!this)
		return {};

	return get_rotation(this);
}

float structures::player_t::get_fov_to(structures::tarkov::bones bone, const structures::unity::vector2_t& rotation) const
{

	const auto hitbox_pos = this->get_bone_transform(bone)->get_position();

	const auto angle = mgrs.feature_mgr.globals.get_camera_pos().get_angle_to(hitbox_pos);

	const auto fov = rotation.get_angle_dist(angle);

	return fov;
}

structures::abstract_hands_controller* structures::player_t::get_abstract_hands_controller() const
{
	static const auto hands_controller_o = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_field("_handsController")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<structures::abstract_hands_controller**>((std::uintptr_t)this + hands_controller_o);
}

structures::procedural_weapon_animation_t* structures::player_t::get_procedural_weapon_animation() const
{
	static const auto get_procedural_weapon_animation = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_method_from_name("get_ProceduralWeaponAnimation", -1)
		->get_function_pointer< structures::procedural_weapon_animation_t* (__fastcall*)(const void* player) >();

	if (!this)
		return nullptr;

	return get_procedural_weapon_animation(this);
}

structures::unity::transform_t* structures::player_t::get_bone_transform(structures::tarkov::bones index) const
{
	if (!this)
		return nullptr;

	return get_player_body()->get_skeleton_root_joint()->values->at(index);
}

structures::shot_effector_t* structures::procedural_weapon_animation_t::get_shot_effector() const
{
	static const auto shot_effector = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Animations", "ProceduralWeaponAnimation")->get_field("Shootingg")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<shot_effector_t**>(reinterpret_cast<std::uintptr_t>(this) + shot_effector);
}

structures::breath_effector_t* structures::procedural_weapon_animation_t::get_breath_effector() const
{
	static const auto breath_effector = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Animations", "ProceduralWeaponAnimation")->get_field("Breath")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<breath_effector_t**>(reinterpret_cast<std::uintptr_t>(this) + breath_effector);
}

structures::firearm_controller_t* structures::procedural_weapon_animation_t::get_firearm_controller() const
{
	static const auto firearm_controller = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Animations", "ProceduralWeaponAnimation")->get_field("\uE011")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<firearm_controller_t**>(reinterpret_cast<std::uintptr_t>(this) + firearm_controller);
}

structures::unity::transform_t* structures::player_t::get_fireport() const
{
	static const auto get_fireport = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_method_from_name("get_Fireport", -1)
		->get_function_pointer< structures::bifacial_transform_t* (__fastcall*)(const void* player) >();

	if (!this)
		return nullptr;

	if (const auto bifacial_tranform = get_fireport(this))
		return bifacial_tranform->original;

	return nullptr;
}

structures::unity::vector2_t structures::player_t::get_health() const
{
	structures::unity::vector2_t end_health{};
	if (!this) return end_health;

	static const auto get_health_controller = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_method_from_name("get_HealthController", -1)
		->get_function_pointer< std::uintptr_t(__fastcall*)(const void* player) >();

	std::uintptr_t health_controller = get_health_controller(this);
	if (!health_controller) return end_health;

	std::uintptr_t dict = *reinterpret_cast<std::uintptr_t*>(health_controller + 0x58); //this should never change //updated 8/11/2022 it actually changed lol
	if (!dict) return end_health;

	//everything below here shouldnt change unless System.Collections.Generic.Dictionary changes, which it shouldnt LOL
	std::uintptr_t entries = *reinterpret_cast<std::uintptr_t*>(dict + 0x18);
	if (!entries) return end_health;

	for (int v = 0; v < 7; v++) {
		std::uintptr_t entry = *reinterpret_cast<std::uintptr_t*>(entries + 0x30 + (0x18 * v));
		if (!entry) continue;

		std::uintptr_t val = *reinterpret_cast<std::uintptr_t*>(entry + 0x10);
		if (!val) continue;

		end_health.x += *reinterpret_cast<float*>(val + 0x10); //current
		end_health.y += *reinterpret_cast<float*>(val + 0x14); //maximum
	}

	return end_health;
}

structures::profile_t* structures::player_t::get_profile() const
{

	static const auto get_player_profile = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_method_from_name("get_Profile", -1)
		->get_function_pointer< structures::profile_t* (__fastcall*)(const void* player) >();

	if (!this)
		return 0;

	return get_player_profile(this);
}

void structures::breath_effector_t::set_intensity(float value) const
{
	static const auto intensity = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Animations", "BreathEffector")->get_field("Intensity")->get_offset();

	if (!this)
		return;

	*reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(this) + intensity) = value;
}

std::vector< structures::unity::renderer_t* > structures::player_body_t::get_renderers() const
{
	if (!this)
		return { };

	return body_renderers
		->renderers->vec();
}

/* Todo make propery dictionary container */
std::vector< structures::lodded_skin_t* > structures::player_body_t::get_body_skins() const
{
	if (!this)
		return { };

	const auto body_skins = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38);

	if (!body_skins)
		return { };

	const auto entry_start = *reinterpret_cast<std::uintptr_t*>(body_skins + 0x18);

	const auto count = *reinterpret_cast<std::uint32_t*>(body_skins + 0x40);

	std::vector< structures::lodded_skin_t* > lodded_skins;

	for (auto i = 0u; i < count; ++i)
	{
		if (const auto lodded_skin = *reinterpret_cast<structures::lodded_skin_t**>(entry_start + 0x30 + 0x18 * i))
		{
			lodded_skins.push_back(lodded_skin);
		}
	}

	return lodded_skins;
}

structures::skeleton_t* structures::player_body_t::get_skeleton_root_joint() const
{
	static const auto skeleton_root_joint = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "PlayerBody")->get_field("SkeletonRootJoint")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<structures::skeleton_t**>(reinterpret_cast<std::uintptr_t>(this) + skeleton_root_joint);
}

structures::unity::shader_t* structures::unity::shader_t::find_shader(const str_t& name)
{
	static const auto find_game_object = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Shader")->get_method_from_name("Find", -1)
		->get_function_pointer< structures::unity::shader_t* (__fastcall*)(str_t) >();

	return find_game_object(name);
}

std::uintptr_t structures::lodded_skin_t::get_renderers() const
{
	//static const auto get_renderers = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
	//	->get_class( "UnityEngine", "Shader" )->get_method_from_name( "Find", -1 )
	//		->get_function_pointer< structures::unity::shader_t* ( __fastcall* )( str_t ) >( );

	return std::uintptr_t();
}

structures::unity::vector3_t structures::unity::transform_t::get_position() const
{
	static const auto method = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Transform")->get_method_from_name("get_position_Injected", -1)
		->get_function_pointer< void(__fastcall*)(const transform_t* transform, vector3_t&) >();

	if (!this)
		return { };

	vector3_t vec;

	method(this, vec);

	return vec;
}

structures::unity::transform_t* structures::unity::camera_t::get_transform()
{
	static const auto method = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Component")->get_method_from_name("get_transform", -1)
		->get_function_pointer< transform_t* (__fastcall*)(const void*) >();

	if (!this)
		return nullptr;

	return method(this);
}

structures::unity::camera_t* structures::unity::camera_t::get()
{
	static const auto get_main_camera = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Camera")->get_method_from_name("get_main", -1)
		->get_function_pointer< structures::unity::camera_t* (__fastcall*)() >();

	return get_main_camera();
}

float structures::unity::camera_t::get_fov() const
{
	const auto get_fov = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Camera")->get_method_from_name("get_fieldOfView", -1)
		->get_function_pointer< float(__fastcall*)(const structures::unity::camera_t*) >();

	if (!this)
		return 0;

	return get_fov(this);
}

float structures::unity::camera_t::get_aspect_ratio() const
{
	const auto get_aspect_ratio = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Camera")->get_method_from_name("get_aspect", -1)
		->get_function_pointer< float(__fastcall*)(const structures::unity::camera_t*) >();

	if (!this)
		return 0;

	return get_aspect_ratio(this);
}

std::optional< structures::unity::vector3_t > structures::unity::camera_t::world_to_screen(const vector3_t& in) const
{
	static const auto world_to_screen_point = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Camera")->get_method_from_name("WorldToScreenPoint_Injected", -1)
		->get_function_pointer< void(__fastcall*)(const structures::unity::camera_t*, vector3_t, int, vector3_t&) >();

	if (!this)
		return std::nullopt;

	vector3_t vec;

	world_to_screen_point(this, in, 2, vec);

	if (vec.z < 0.098f)
		return std::nullopt;

	vec.y = IM_FLOOR(screen_t::get_height() - vec.y);
	vec.x = IM_FLOOR(vec.x);

	return vec;
}

structures::unity::transform_t* structures::unity::game_object_t::get_transform() const
{
	static const auto get_transform = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "GameObject")->get_method_from_name("get_transform", -1)
		->get_function_pointer< structures::unity::transform_t* (__fastcall*)(const void* object) >();

	if (!this)
		return nullptr;

	return get_transform(this);
}

float structures::unity::screen_t::get_height()
{
	static const auto screen_height = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Screen")->get_method_from_name("get_height", -1)
		->get_function_pointer< int(__fastcall*)() >();

	return screen_height();
}

float structures::unity::screen_t::get_width()
{
	static const auto screen_width = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Screen")->get_method_from_name("get_width", -1)
		->get_function_pointer< int(__fastcall*)() >();

	return screen_width();
}

structures::unity::vector2_t structures::unity::screen_t::get_dimensions()
{
	return { get_width(), get_height() };
}

structures::unity::rigid_body_t* structures::throwable_t::get_rigid_body() const
{
	static const auto rigid_body = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "Throwable")->get_field("Rigidbody")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<structures::unity::rigid_body_t**>(reinterpret_cast<std::uintptr_t>(this) + rigid_body);
}

structures::unity::vector3_t structures::unity::rigid_body_t::get_position() const
{
	static const auto method = mono::structures::image_t::get_image_by_name("UnityEngine.PhysicsModule")
		->get_class("UnityEngine", "Rigidbody")->get_method_from_name("get_position_Injected", -1)
		->get_function_pointer< void(__fastcall*)(const void* rigid_body, vector3_t&) >();

	if (!this)
		return { };

	vector3_t vec;

	method(this, vec);

	return vec;
}

bool structures::unity::physics_t::linecast(vector3_t start, vector3_t end, raycast_hit_t* hitinfo, int layermask)
{
	static const auto method = mono::structures::image_t::get_image_by_name("UnityEngine.PhysicsModule")
		->get_class("UnityEngine", "Physics")->get_method_from_name("Linecast", 5)
		->get_function_pointer< bool(__fastcall*)(vector3_t start, vector3_t end, raycast_hit_t* hitinfo, int layermask, int) >();

	return method(start, end, hitinfo, layermask, 2);
}

structures::unity::transform_t* structures::unity::raycast_hit_t::get_transform() const
{
	static const auto method = mono::structures::image_t::get_image_by_name("UnityEngine.PhysicsModule")
		->get_class("UnityEngine", "RaycastHit")->get_method_from_name("get_transform", -1)
		->get_function_pointer< transform_t* (__fastcall*)(const void*) >();

	if (!this)
		return nullptr;

	return method(this);
}

structures::unity::str_t* structures::profile_t::get_nickname() const
{
	static const auto get_nickname = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Profile")->get_method_from_name("get_Nickname", -1)
		->get_function_pointer< structures::unity::str_t* (__fastcall*)(const void* profile) >();

	if (!this)
		return nullptr;

	return get_nickname(this);
}

structures::profile_info_t* structures::profile_t::get_profile_info() const
{
	static const auto offset = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Profile")->get_field("Info")->get_offset();

	if (!this)
		return 0;

	return *reinterpret_cast<structures::profile_info_t**>((uint64_t)this + offset);
}

structures::profile_stats_t* structures::profile_t::get_stats() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Stats")->get_offset();

	return *reinterpret_cast<structures::profile_stats_t**>((std::uintptr_t)this + offset);
}

structures::unity::str_t* structures::profile_info_t::get_main_nickname() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("MainProfileNickname")->get_offset();

	return *reinterpret_cast<structures::unity::str_t**>((std::uintptr_t)this + offset);
}

std::string structures::profile_settings_t::get_scav_name() const
{
#define SCAV_NAME_MACRO(type, name) case type: return name
	switch (this->get_spawn_type()) {
		SCAV_NAME_MACRO(no_spawn_type, "nil");
		SCAV_NAME_MACRO(marksman, "s-scav");
		SCAV_NAME_MACRO(assault, "scav");
		SCAV_NAME_MACRO(bossTest, "boss test");
		SCAV_NAME_MACRO(bossBully, "rashalla");
		SCAV_NAME_MACRO(followerTest, "test follower");
		SCAV_NAME_MACRO(followerBully, "goon");
		SCAV_NAME_MACRO(bossKilla, "killa");
		SCAV_NAME_MACRO(bossKojaniy, "shturman");
		SCAV_NAME_MACRO(followerKojaniy, "goon");
		SCAV_NAME_MACRO(pmcBot, "raider");
		SCAV_NAME_MACRO(cursedAssault, "cursed");
		SCAV_NAME_MACRO(bossGluhar, "gluhar");
		SCAV_NAME_MACRO(followerGluharAssault, "goon-a");
		SCAV_NAME_MACRO(followerGluharSecurity, "goon-se");
		SCAV_NAME_MACRO(followerGluharScout, "goon-sc");
		SCAV_NAME_MACRO(followerGluharSnipe, "goon-sn");
		SCAV_NAME_MACRO(followerSanitar, "goon");
		SCAV_NAME_MACRO(bossSanitar, "sanitar");
		SCAV_NAME_MACRO(test, "test");
		SCAV_NAME_MACRO(assaultGroup, "assault");
		SCAV_NAME_MACRO(sectantWarrior, "sectant-w");
		SCAV_NAME_MACRO(sectantPriest, "sectant-p");
		SCAV_NAME_MACRO(bossTagilla, "tagilla");
		SCAV_NAME_MACRO(followerTagilla, "goon");
		SCAV_NAME_MACRO(exUsec, "rogue");
		SCAV_NAME_MACRO(gifter, "gifter");
		SCAV_NAME_MACRO(bossKnight, "knight");
		SCAV_NAME_MACRO(followerBigPipe, "bigpipe");
		SCAV_NAME_MACRO(followerBirdEye, "birdeye");
	default:
		return "nil";
	}
#undef SCAV_NAME_MACRO
}

structures::e_player_side structures::profile_info_t::get_side() const
{
	if (!this)
		return e_player_side::no_side;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Side")->get_offset();

	return *reinterpret_cast<structures::e_player_side*>((std::uintptr_t)this + offset);
}

bool structures::profile_info_t::is_scav() const
{
	const auto side = this->get_side();
	return side == structures::e_player_side::savage;
}

structures::e_member_category structures::profile_info_t::get_member_category() const
{
	if (!this)
		return structures::e_member_category::no_category;

	static const auto offset = mono::functions::object_get_class(this)->get_field("MemberCategory")->get_offset();

	return *reinterpret_cast<structures::e_member_category*>((std::uintptr_t)this + offset);
}

std::string structures::profile_info_t::get_member_category_name() const
{
	if (!this)
		return {};

	std::string end_str = {};
#define CATEGORY_NAME_MACRO(type, name) case type: return name
	switch (this->get_member_category()) {
		CATEGORY_NAME_MACRO(structures::e_member_category::no_category, {});
		CATEGORY_NAME_MACRO(structures::e_member_category::developer, "dev");
		CATEGORY_NAME_MACRO(structures::e_member_category::edge_of_darkness, "eod");
		CATEGORY_NAME_MACRO(structures::e_member_category::trader, "trader");
		CATEGORY_NAME_MACRO(structures::e_member_category::group, "group");
		CATEGORY_NAME_MACRO(structures::e_member_category::system, "admin");
		CATEGORY_NAME_MACRO(structures::e_member_category::chat_mod, "chat_mod");
		CATEGORY_NAME_MACRO(structures::e_member_category::chat_mod_with_permban, "chat_mod_perms");
		CATEGORY_NAME_MACRO(structures::e_member_category::unit_test, "unit_test");
		CATEGORY_NAME_MACRO(structures::e_member_category::sherpa, "sherpa");
		CATEGORY_NAME_MACRO(structures::e_member_category::emissary, "emissary");
	default:
		return {};
	}
#undef CATEGORY_NAME_MACRO

}

std::int32_t structures::profile_info_t::get_registration_date() const
{
	if (!this)
		return 0;

	static const auto offset = mono::functions::object_get_class(this)->get_field("RegistrationDate")->get_offset();

	return *reinterpret_cast<std::int32_t*>((std::uintptr_t)this + offset);
}

int32_t structures::profile_info_t::get_level() const
{
	if (!this)
		return 0;

	static const auto get_level = mono::functions::object_get_class(this)->get_method_from_name("get_Level", -1)->get_function_pointer<int(*)(const structures::profile_info_t*)>();

	return get_level(this);
}

structures::profile_settings_t* structures::profile_info_t::get_settings() const
{
	if (!this)
		return 0;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Settings")->get_offset();

	return *reinterpret_cast<structures::profile_settings_t**>((std::uintptr_t)this + offset);
}

structures::unity::str_t* structures::profile_info_t::get_nickname() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Nickname")->get_offset();

	return *reinterpret_cast<structures::unity::str_t**>((std::uintptr_t)this + offset);
}

structures::exit_trigger_settings_t* structures::exfiltration_point_t::get_settings() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "ExfiltrationPoint")->get_field("Settings")->get_offset();

	return *reinterpret_cast<structures::exit_trigger_settings_t**>((std::uintptr_t)this + offset);
}

structures::e_exfiltration_status structures::exfiltration_point_t::get_status() const
{
	if (!this)
		return structures::e_exfiltration_status::not_present;

	static const auto offset = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "ExfiltrationPoint")->get_field("_status")->get_offset();

	return *reinterpret_cast<structures::e_exfiltration_status*>((std::uintptr_t)this + offset);
}

structures::unity::str_t* structures::exit_trigger_settings_t::get_name() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "ExitTriggerSettings")->get_field("Name")->get_offset();

	return *reinterpret_cast<structures::unity::str_t**>((std::uintptr_t)this + offset);
}

structures::unity::transform_t* structures::unity::component_t::get_transform() const
{
	static const auto method = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Component")->get_method_from_name("get_transform", -1)
		->get_function_pointer< transform_t* (__fastcall*)(const void*) >();

	if (!this)
		return nullptr;

	return method(this);
}

structures::item_t* structures::firearm_controller_t::get_item() const
{
	static const auto method = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "Player")->get_nested_class("ItemHandsController")->get_method_from_name("GetItem", -1)
		->get_function_pointer< structures::item_t* (__fastcall*)(const void*) >();

	if (!this)
		return nullptr;

	return method(this);
}

structures::unity::str_t* structures::item_t::localized_short_name() const
{
	if (!this)
		return nullptr;

	static const auto localized_short_name = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "\uE6F2")->get_method_from_name("LocalizedShortName", -1) //updated 8/11/2022
		->get_function_pointer< structures::unity::str_t* (__fastcall*)(const structures::item_t* item) >();

	return localized_short_name(this);
}

structures::item_template_t* structures::item_t::get_item_template() const
{
	//static const auto item_template_offset = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
	//	->get_class("EFT.InventoryLogic", "Item")->get_field("<Template>k__BackingField")->get_offset();

	if (!utilities::valid_mem(this))
		return nullptr;

	return *reinterpret_cast<structures::item_template_t**>((uint64_t)this + 0x40);
}

int structures::item_t::get_price() const
{
	if (!this)
		return 0;

	if (const auto item_template = this->get_item_template()) {
		if (const auto template_id = item_template->get_template_id()) {
			if (game::loot_data::data.contains(template_id->get())) {
				auto val = game::loot_data::data.at(template_id->get());

				return val;
			}
		}
	}
	return 0;
}

int structures::item_t::get_chamber_count() const
{
	static const auto get_chamber_count = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.InventoryLogic", "Weapon")->get_method_from_name("get_ChamberAmmoCount", -1)
		->get_function_pointer< int(__fastcall*)(const structures::item_t* item) >();

	if (!this)
		return 0;

	return get_chamber_count(this);
}

int structures::item_t::get_max_magazine_count() const
{
	static const auto get_max_magazine_count = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.InventoryLogic", "Weapon")->get_method_from_name("GetMaxMagazineCount", -1)
		->get_function_pointer< int(__fastcall*)(const structures::item_t* item) >();

	if (!this)
		return 0;

	return get_max_magazine_count(this);
}

int structures::item_t::get_current_magazine_count() const
{
	static const auto get_magazine_count = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.InventoryLogic", "Weapon")->get_method_from_name("GetCurrentMagazineCount", -1)
		->get_function_pointer< int(__fastcall*)(const structures::item_t* item) >();

	if (!this)
		return 0;

	return get_magazine_count(this);
}

bool structures::unity::input_t::get_key(e_keycodes key_code)
{
	static const auto get_key_int = mono::structures::image_t::get_image_by_name("UnityEngine.InputLegacyModule")
		->get_class("UnityEngine", "Input")->get_method_from_name("GetKeyInt", -1)
		->get_function_pointer< bool(__fastcall*)(int32_t key) >();

	return get_key_int(key_code);
}

structures::item_t* structures::loot_item_t::get_item() const
{
	static const auto get_item = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "LootItem")->get_method_from_name("get_Item", -1)
		->get_function_pointer< structures::item_t* (__fastcall*)(const void*) >();

	if (!this)
		return nullptr;

	return get_item(this);
}

structures::unity::game_object_t* structures::loot_item_t::get_game_object() const
{
	static const auto get_game_object = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Component")->get_method_from_name("get_gameObject", -1)
		->get_function_pointer< structures::unity::game_object_t* (__fastcall*)(const void*) >();

	if (!this)
		return nullptr;

	return get_game_object(this);
}

structures::unity::str_t* structures::item_template_t::get_template_id() const
{
	static const auto template_id = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.InventoryLogic", "ItemTemplate")->get_field("_id")->get_offset();

	if (!this)
		return nullptr;

	return *reinterpret_cast<structures::unity::str_t**>((uintptr_t)this + template_id);
}

structures::e_wild_spawn_type structures::profile_settings_t::get_spawn_type() const
{
	if (!this)
		return structures::e_wild_spawn_type::no_spawn_type;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Role")->get_offset();

	return *reinterpret_cast<structures::e_wild_spawn_type*>((std::uintptr_t)this + offset);
}

int structures::profile_stats_t::get_total_session_experience() const
{
	if (!this)
		return 0;

	static const auto offset = mono::functions::object_get_class(this)->get_field("TotalSessionExperience")->get_offset();

	return *reinterpret_cast<int*>((std::uintptr_t)this + offset);
}

structures::root_item_t* structures::item_owner_t::get_root() const
{
	if (!this)
		return nullptr;

	static const auto get_root = mono::functions::object_get_class(this)->get_method_from_name("get_Root", -1)
		->get_function_pointer< structures::root_item_t* (__fastcall*)(const void*) >();

	return get_root(this);
}

structures::item_owner_t* structures::lootable_container_t::get_item_owner() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("ItemOwner")->get_offset();

	return *reinterpret_cast<structures::item_owner_t**>((std::uintptr_t)this + offset);
}

structures::unity::array_t<structures::root_item_grid_t*>* structures::root_item_t::get_grids() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Grids")->get_offset();

	return *reinterpret_cast<structures::unity::array_t<structures::root_item_grid_t*>**>((std::uintptr_t)this + offset);
}

structures::item_collection_t* structures::root_item_grid_t::get_item_collection() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("\uE002")->get_offset();

	return *reinterpret_cast<structures::item_collection_t**>((std::uintptr_t)this + offset);
}

structures::unity::list_t<structures::item_t*>* structures::item_collection_t::get_items() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("\uE001")->get_offset();

	return *reinterpret_cast<structures::unity::list_t<structures::item_t*>**>((std::uintptr_t)this + offset);
}

structures::inventory_t* structures::inventory_controller_t::get_inventory() const
{
	if (!this)
		return 0;

	static const auto get_inventory = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "\uE9CC")->get_method_from_name("get_Inventory", -1)
		->get_function_pointer< structures::inventory_t* (__fastcall*)(const void*) >();

	return get_inventory(this);
}

structures::equipment_t* structures::inventory_t::get_equipment() const
{
	if (!this)
		return nullptr;

	static const auto offset = mono::functions::object_get_class(this)->get_field("Equipment")->get_offset();

	return *reinterpret_cast<equipment_t**>((std::uintptr_t)this + offset);
}

structures::slot_t* structures::equipment_t::get_slot(EquipmentSlot slot) const
{
	if (!this)
		return 0;

	static const auto get_slot = mono::functions::object_get_class(this)
		->get_method_from_name("GetSlot", -1)
		->get_function_pointer< structures::slot_t* (__fastcall*)(const void* equipment, EquipmentSlot slot) >();

	return get_slot(this, slot);
}

structures::item_t* structures::slot_t::get_contained_item() const
{
	if (!this)
		return 0;

	static const auto get_contained_item = mono::functions::object_get_class(this)
		->get_method_from_name("get_ContainedItem", -1)
		->get_function_pointer < structures::item_t* (__fastcall*)(const void* rcx) >();

	return get_contained_item(this);
}
