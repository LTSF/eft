#pragma once

#include <dependencies/common_includes.hpp>
#include <codecvt>

#define DEG2RAD(x) (x * (3.14159265358979323846f / 180.f))

namespace structures
{
	namespace unity
	{

		struct vector2_t
		{
			float x, y;

			bool empty() {
				return x == 0.f && y == 0.f;
			}

			vector2_t operator-(const vector2_t& vec) const
			{
				return { x - vec.x, y - vec.y };
			}

			vector2_t operator+(const vector2_t& vec) const
			{
				return { x + vec.x, y + vec.y };
			}

			vector2_t operator*(float val) const
			{
				return { x * val, y * val };
			}

			vector2_t operator/(float val) const
			{
				return { x / val, y / val };
			}

			float magnitude()
			{
				return std::sqrtf((x * x) + (y * y));
			}

			float get_angle_dist(const vector2_t& angle) const {
				vector2_t diff = *this - angle;
				if (diff.x < -180.f)
					diff.x += 360.f;
				if (diff.x > 180.f)
					diff.x -= 360.f;
				return fabsf(diff.magnitude());
			}
		};

		struct vector3_t
		{
			float x, y, z;

			vector3_t(float x, float y, float z)
				: x(x), y(y), z(z)
			{}


			vector3_t()
				: x(0), y(0), z(0)
			{}

			const float dot(const vector3_t& vec) const
			{
				return x * vec.x + y * vec.y + z * vec.z;
			}

			const float distance(vector3_t vector) const
			{
				return sqrtf(powf(vector.x - x, 2.0) + powf(vector.y - y, 2.0) + powf(vector.z - z, 2.0));
			}

			const vector3_t operator*(float value) const
			{
				return { x * value, y * value, z * value };
			}

			const float squared() const
			{
				return x * x + y * y + z * z;
			}

			vector3_t operator-(const vector3_t& vec) const
			{
				return { x - vec.x, y - vec.y, z - vec.z };
			}

			vector3_t operator+(const vector3_t& vec) const
			{
				return { x + vec.x, y + vec.y, z + vec.z };
			}

			vector3_t operator*(const vector3_t& vec) const
			{
				return { x * vec.x, y * vec.y, z * vec.z };
			}

			vector3_t operator/(const vector3_t& vec) const
			{
				return { x / vec.x, y / vec.y, z / vec.z };
			}

			float magnitude()
			{
				return std::sqrtf((x * x) + (y * y) + (z * z));
			}

			vector3_t normalized()
			{
				float len = magnitude();
				return vector3_t(x / len, y / len, z / len);
			}

			bool empty() const
			{
				return x == 0.f && y == 0.f && z == 0.f;
			}

			vector2_t get_angle_to(const vector3_t& dest) const {
				vector3_t diff = *this - dest;
				vector2_t ret;

				float length = diff.magnitude();
				ret.y = asinf(diff.y / length);
				ret.x = -atan2f(diff.x, -diff.z);

				return ret * (180.f / 3.141592654f);
			}
		};

		struct vector4_t
		{
			float x, y, z, w;
		};

		struct screen_t
		{
			static float get_height();
			static float get_width();
			static vector2_t get_dimensions();
		};

		struct component_t {
			struct transform_t* get_transform() const;
		};

		struct camera_t
		{
			struct transform_t* get_transform();

			static camera_t* get();

			float get_fov() const;

			float get_aspect_ratio() const;

			std::optional< vector3_t > world_to_screen(const vector3_t& in) const;
		};

		struct str_t
		{
			std::uint8_t pad[0x10];
			std::uint32_t sz;

			wchar_t buff[std::numeric_limits< std::int8_t >::max()];

			str_t(const wchar_t* const string)
				: sz{ static_cast<std::uint32_t>(std::wcslen(string)) }
			{
				std::wcscpy(buff, string);
			}

			std::wstring get() {
				if (!this)
					return {};
				return buff;
			}

			std::string convert() {
				if (!this)
					return "empty str";

				int32_t size = *reinterpret_cast<int32_t*>((std::uintptr_t)this + 0x10);
				wchar_t buf[64] = { '\0' };
				std::memcpy(buf, &this->buff, size * 2);
				std::string u8_conv = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(buf);

				return u8_conv;
			}
		};

		struct asset_bundle_t
		{

		};

		struct shader_t
		{
			static shader_t* find_shader(const str_t& name);
		};

		struct material_t
		{
			void set_shader(void* shader) const;

			shader_t* get_shader() const;

			struct color_t
			{
				float r, g, b, a;
			};
			void set_color(const str_t& name, const color_t& color) const;

			void set_float(const str_t& name, float) const;

			void set_int(const str_t& name, int i) const;
		};

		struct renderer_t
		{
			material_t* get_material() const;
		};

		struct skinned_mesh_renderer_t
		{
			std::uint8_t pad[0x10];
			renderer_t* renderer;
		};

		struct transform_t
		{
			vector3_t get_position() const;
		};

		struct class_object_t
		{
			std::uint8_t pad[0x18];
			std::uint8_t* object_reference;
		};

		struct rigid_body_t
		{
			vector3_t get_position() const;
		};

		struct game_object_t
		{
			std::uint8_t pad[0x30];
			class_object_t* class_object;

			transform_t* get_transform() const;
		};

		struct base_object_t
		{
			std::uint8_t pad[0x10];
			game_object_t* game_object;

			game_object_t* get_game_object() const;

			static base_object_t* find(const str_t& name);
		};

		template< typename t >
		struct list_t
		{
			std::uint8_t pad[0x10];
			std::uint8_t* start;
			std::uint32_t sz;

			std::uint32_t size() const
			{
				if (!this)
					return 0;
				return sz;
			}

			bool valid() const
			{
				return this && start;
			}

			t at(std::uint32_t index) const
			{
				if (!valid())
					return { };

				return *reinterpret_cast<t*>(start + 0x20 + (index * sizeof(std::uintptr_t)));
			}

			std::vector< t > vec() const
			{
				if (!valid())
					return { };

				return { reinterpret_cast<t*>(start + 0x20), reinterpret_cast<t*>(start + 0x20 + (sz * sizeof(std::uintptr_t))) };
			}
		};

		/* Renderer[] */
		template < typename t >
		struct array_t
		{
			std::uint8_t pad[0x18];
			std::uint32_t count; /* 0x18 */
			std::uint8_t pad1[0x4]; /* 0x1C */
			t start; /* 0x20 */

			bool valid() const
			{
				return this && start;
			}

			t at(std::uint32_t index) const
			{
				if (!valid())
					return { };

				return &start[index];
			}

			std::vector< t > vec() const
			{
				if (!valid())
					return { };

				return { &start, &start + count };
			}
		};

		template< typename key_t, typename value_t >
		struct dictionary_t
		{

		};

		enum e_keycodes {
			mouse4 = 0x147,
		};

		struct input_t {
			static bool get_key(e_keycodes key_code);
		};

		struct raycast_hit_t
		{
			std::uint8_t pad[0x10];
			vector3_t point;
			vector3_t normal;
			std::uint32_t faceid;
			float distance;
			vector2_t uv;
			int collider;

			transform_t* get_transform() const;
		};

		struct physics_t
		{
			static bool linecast(vector3_t start, vector3_t end, raycast_hit_t* hitinfo, int layermask);
		};
	}

	namespace tarkov
	{
		enum layers
		{
			DefaultLayer = 1 << 0,
			TransparentFx = 1 << 1,
			IgnoreRaycast = 1 << 2,
			Water = 1 << 4,
			Ui = 1 << 5,
			Player_ = 1 << 8,
			DoorLowPoly = 1 << 9,
			PlayerCollisionTest = 1 << 10,
			Terrain = 1 << 11,
			HighPoly = 1 << 12,
			Triggers = 1 << 13,
			DisabledCullingObject = 1 << 14,
			Loot = 1 << 15,
			HitCollider = 1 << 16,
			PlayerRenders = 1 << 17,
			LowPoly = 1 << 18,
			WeaponPreview = 1 << 19,
			Shells = 1 << 20,
			CullingMask = 1 << 21,
			Interactive = 1 << 22,
			DeadBody = 1 << 23,
			RainDrops = 1 << 24,
			MenuEnvironment = 1 << 25,
			Foliage = 1 << 26,
			PlayerSpiritAura = 1 << 27,
			Sky = 1 << 28,
			LevelBorder = 1 << 29
		};

		enum bones : std::int32_t
		{
			root = 13,

			l_thigh_1 = 15,
			l_thigh_2 = 16,

			r_thigh_1 = 20,
			r_thigh_2 = 21,

			l_calf = 17,
			r_calf = 22,

			l_foot = 18,
			r_foot = 23,

			pelvis = 14,
			ribcage = 66,

			spine_1 = 29,
			spine_2 = 36,
			spine_3 = 37,

			l_collarbone = 89,
			r_collarbone = 110,

			l_palm = 94,
			r_palm = 115,

			l_upperarm = 111,
			r_upperarm = 90,

			l_forearm_1 = 91,
			l_forearm_2 = 92,
			l_forearm_3 = 93,

			r_forearm_1 = 112,
			r_forearm_2 = 113,
			r_forearm_3 = 114,

			head = 133,
			neck = 132,
		};
	}

	struct item_collection_t
	{
		unity::list_t<struct item_t*>* get_items() const;
	};

	struct root_item_grid_t
	{
		item_collection_t* get_item_collection() const;
	};

	struct root_item_t
	{
		unity::array_t<root_item_grid_t*>* get_grids() const;
	};

	struct item_owner_t
	{
		root_item_t* get_root() const;
	};

	struct lootable_container_t : unity::component_t
	{
		item_owner_t* get_item_owner() const;
	};

	struct item_template_t {
		unity::str_t* get_template_id() const;
	};

	struct item_t : unity::component_t {
		unity::str_t* localized_short_name() const;

		item_template_t* get_item_template() const;

		int get_price() const;


		/*EFT.InventoryLogic.Weapon*/
		int get_chamber_count() const;
		int get_max_magazine_count() const;
		int get_current_magazine_count() const;
	};

	enum loot_command_status_t : std::int32_t
	{
		Begin,
		Succeed,
		Failed
	};

	struct loot_command_t
	{
		std::uint8_t pad[0x10];
		void* item;
		loot_command_status_t status;
		void* item_addr;
	};

	struct loot_item_t : unity::component_t {
		item_t* get_item() const;

		unity::game_object_t* get_game_object() const;
	};

	struct loot_entry_t {
		structures::loot_item_t* loot_item;
		structures::item_t* item;
		structures::unity::str_t* id;
		std::int32_t price;

		loot_entry_t(structures::loot_item_t* loot_item, structures::item_t* item, std::int32_t price) : loot_item(loot_item), item(item), price(price) {};
		loot_entry_t() {};

		bool operator==(const loot_entry_t& l) const { return loot_item == l.loot_item; };
	};

	struct exit_trigger_settings_t {
		unity::str_t* get_name() const;
	};

	enum e_exfiltration_status {
		not_present = 1,
		uncompleted_requirements,
		countdown,
		regular_mode,
		pending,
		awaits_manual_activation
	};

	struct exfiltration_point_t : unity::component_t {
		exit_trigger_settings_t* get_settings() const;

		e_exfiltration_status get_status() const;
	};

	struct shot_effector_t
	{
		void set_recoil_strength(float value) const;
	};

	struct breath_effector_t
	{
		void set_intensity(float value) const;
	};

	struct firearm_controller_t
	{
		item_t* get_item() const;
	};

	struct bifacial_transform_t
	{
		std::uint8_t pad[0x10];

		unity::transform_t* original;
	};

	struct player_bones_t
	{

	};

	enum e_wild_spawn_type
	{
		no_spawn_type = 0,
		marksman = 1,
		assault = 2,
		bossTest = 4,
		bossBully = 8,
		followerTest = 16,
		followerBully = 32,
		bossKilla = 64,
		bossKojaniy = 128,
		followerKojaniy = 256,
		pmcBot = 512,
		cursedAssault = 1024,
		bossGluhar = 2048,
		followerGluharAssault = 4096,
		followerGluharSecurity = 8192,
		followerGluharScout = 16384,
		followerGluharSnipe = 32768,
		followerSanitar = 65536,
		bossSanitar = 131072,
		test = 262144,
		assaultGroup = 524288,
		sectantWarrior = 1048576,
		sectantPriest = 2097152,
		bossTagilla = 4194304,
		followerTagilla = 8388608,
		exUsec = 16777216,
		gifter = 33554432,
		bossKnight = 67108864,
		followerBigPipe = 134217728,
		followerBirdEye = 268435456
	};

	struct profile_settings_t {
		e_wild_spawn_type get_spawn_type() const;

		std::string get_scav_name() const;
	};

	enum e_player_side {
		no_side = 0,
		usec = 1 << 0,
		bear = 1 << 1,
		savage = 1 << 2,
	};

	struct profile_stats_t {
		int get_total_session_experience() const;
	};

	enum e_member_category {
		no_category = 0,
		developer = 1,
		edge_of_darkness = 2,
		trader = 4,
		group = 8,
		system = 16,
		chat_mod = 32,
		chat_mod_with_permban = 64,
		unit_test = 128,
		sherpa = 256,
		emissary = 512
	};

	struct profile_info_t
	{
		unity::str_t* get_nickname() const;

		unity::str_t* get_main_nickname() const;
		e_player_side get_side() const;
		bool is_scav() const;

		e_member_category get_member_category() const;
		std::string get_member_category_name() const;

		int32_t get_registration_date() const;

		int32_t get_level() const;

		profile_settings_t* get_settings() const;
	};

	struct profile_t
	{
		unity::str_t* get_nickname() const;

		profile_info_t* get_profile_info() const;

		profile_stats_t* get_stats() const;
	};

	struct procedural_weapon_animation_t
	{
		shot_effector_t* get_shot_effector() const;

		breath_effector_t* get_breath_effector() const;

		firearm_controller_t* get_firearm_controller() const;
	};

	struct skin_t
	{
		std::uint8_t pad[0x20];
		unity::skinned_mesh_renderer_t* skinned_mesh_renderer;
	};

	struct lodded_skin_t
	{
		std::uintptr_t get_renderers() const;
	};

	struct body_renderers_t
	{
		char pad_0000[40]; //0x0000
		unity::array_t< unity::renderer_t* >* renderers; //0x0028
	};

	struct skeleton_t
	{
		std::uint8_t pad[0x28];
		unity::list_t< unity::transform_t* >* values;
	};

	struct player_body_t
	{
		std::uint8_t pad[0x48];
		body_renderers_t* body_renderers;

		std::vector< unity::renderer_t* > get_renderers() const;

		std::vector< lodded_skin_t* > get_body_skins() const;

		skeleton_t* get_skeleton_root_joint() const;
	};

	struct abstract_hands_controller {

	};

	struct slot_t
	{
		item_t* get_contained_item() const;
	};

	enum EquipmentSlot
	{
		// Token: 0x0400920A RID: 37386
		FirstPrimaryWeapon,
		// Token: 0x0400920B RID: 37387
		SecondPrimaryWeapon,
		// Token: 0x0400920C RID: 37388
		Holster,
		// Token: 0x0400920D RID: 37389
		Scabbard,
		// Token: 0x0400920E RID: 37390
		Backpack,
		// Token: 0x0400920F RID: 37391
		SecuredContainer,
		// Token: 0x04009210 RID: 37392
		TacticalVest,
		// Token: 0x04009211 RID: 37393
		ArmorVest,
		// Token: 0x04009212 RID: 37394
		Pockets,
		// Token: 0x04009213 RID: 37395
		Eyewear,
		// Token: 0x04009214 RID: 37396
		FaceCover,
		// Token: 0x04009215 RID: 37397
		Headwear,
		// Token: 0x04009216 RID: 37398
		Earpiece,
		// Token: 0x04009217 RID: 37399
		Dogtag,
		// Token: 0x04009218 RID: 37400
		ArmBand
	};

	struct equipment_t
	{
		slot_t* get_slot(EquipmentSlot slot) const;
	};

	struct inventory_t
	{
		equipment_t* get_equipment() const;
	};

	struct inventory_controller_t
	{
		inventory_t* get_inventory() const;
	};

	struct player_t
	{
		inventory_controller_t* get_inventory_controller() const;

		player_body_t* get_player_body() const;

		unity::vector2_t get_rotation() const;

		float get_fov_to(structures::tarkov::bones bone, const structures::unity::vector2_t& rotation) const;

		abstract_hands_controller* get_abstract_hands_controller() const;

		procedural_weapon_animation_t* get_procedural_weapon_animation() const;

		unity::transform_t* get_bone_transform(structures::tarkov::bones index) const;

		unity::transform_t* get_fireport() const;

		unity::vector2_t get_health() const;

		profile_t* get_profile() const;
	};

	struct loot_t
	{

	};

	struct throwable_t
	{
		unity::rigid_body_t* get_rigid_body() const;
	};

	struct game_world_t
	{
		unity::list_t< player_t* >* get_players() const;

		unity::list_t< loot_t* >* get_loot() const;

		unity::list_t< throwable_t* >* get_grenades() const;

		static game_world_t* get();
	};
}