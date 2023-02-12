#pragma once

#include <dependencies/common_includes.hpp>

#include <vector>

const auto mono_module = GetModuleHandleA( "mono-2.0-bdwgc.dll" );

namespace mono
{
	namespace structures
	{
		struct assembly_t
		{
			static std::vector< assembly_t* > get_assemblies( );

			struct image_t* get_image( ) const;
		};

		struct table_info_t
		{
			int64_t get_rows() const;
		};

		struct image_t
		{
			static image_t* get_image_by_name( const char* const name );

			table_info_t* get_table_info( int table_id ) const;

			const char* get_name( ) const;

			bool is( const char* name ) const
			{
				return !std::strcmp( get_name( ), name );
			}

			struct class_t* get_class( const char* const namespace_name, const char* const class_name) const;
		};

		struct domain_t
		{

		};

		struct thread_t
		{

		};

		struct object_t
		{
			class_t* get_class() const;
		};

		struct method_t
		{
			std::uintptr_t get_address( ) const;

			template< typename def_t >
			def_t get_function_pointer( ) const
			{
				return reinterpret_cast< def_t >( get_address( ) );
			}
		};

		struct field_t
		{
			std::uintptr_t get_offset( ) const;
		};

		struct vtable_t
		{
			std::uintptr_t get_static_field_data( ) const;
		};

		struct class_t
		{
			class_t* get_nested_class( const char* class_name );

			method_t* get_method_from_name( const char* method_name, std::int32_t parameter_count ) const;

			field_t* get_field( const char* const name ) const;

			vtable_t* get_vtable( ) const;

			const char* get_namespace_name( ) const;

			const char* get_name( ) const;
		};
	}

	namespace functions
	{
		using get_root_domain_t = structures::domain_t* ( __cdecl* )( );
		const auto get_root_domain = reinterpret_cast< get_root_domain_t >( GetProcAddress( mono_module, "mono_get_root_domain" ) );

		using thread_attach_t = structures::thread_t* ( __cdecl* )( structures::domain_t* domain );
		const auto thread_attach = reinterpret_cast< thread_attach_t >( GetProcAddress( mono_module, "mono_thread_attach" ) );

		using thread_dettach_t = void ( __cdecl* )( structures::thread_t* thread );
		const auto thread_dettach = reinterpret_cast< thread_dettach_t >( GetProcAddress( mono_module, "mono_thread_detach" ) );

		using object_get_class_t = structures::class_t* ( __cdecl* )( const void* object );
		const auto object_get_class = reinterpret_cast< object_get_class_t >( GetProcAddress( mono_module, "mono_object_get_class" ) );

		using compile_method_t = std::uintptr_t( __cdecl* )( const structures::method_t* method );
		const auto compile_method = reinterpret_cast< compile_method_t >( GetProcAddress( mono_module, "mono_compile_method" ) );

		using class_vtable_t = structures::vtable_t*( __cdecl* )( const structures::domain_t*, const structures::class_t* );
		const auto class_vtable = reinterpret_cast< class_vtable_t >( GetProcAddress( mono_module, "mono_class_vtable" ) );

		using vtable_get_static_field_data_t = std::uintptr_t( __cdecl* )( const structures::vtable_t* );
		const auto vtable_get_static_field_data = reinterpret_cast<vtable_get_static_field_data_t>( GetProcAddress( mono_module, "mono_vtable_get_static_field_data" ) );
	
		using domain_iterator_callback_t = void( __cdecl* )( structures::domain_t* domain, void* user_data );

		using domain_foreach_t = void ( __cdecl* )( domain_iterator_callback_t iterator, void* userdata );
		const auto domain_foreach = reinterpret_cast < domain_foreach_t >( GetProcAddress( mono_module, "mono_domain_foreach" ) );

		using assembly_iterator_callback_t = void( __cdecl* )( structures::assembly_t* domain, std::vector< structures::assembly_t* >& user_data );

		using assembly_foreach_t = void( __cdecl* )( assembly_iterator_callback_t iterator, std::vector< structures::assembly_t* >& userdata );
		const auto assembly_foreach = reinterpret_cast < assembly_foreach_t >( GetProcAddress( mono_module, "mono_assembly_foreach" ) );

		using assembly_get_image_t = structures::image_t* ( __cdecl* )( const structures::assembly_t* assembly );
		const auto assembly_get_image = reinterpret_cast < assembly_get_image_t >( GetProcAddress( mono_module, "mono_assembly_get_image" ) );

		using image_get_name_t = const char* ( __cdecl* )( const structures::image_t* image );
		const auto image_get_name = reinterpret_cast < image_get_name_t >( GetProcAddress( mono_module, "mono_image_get_name" ) );

		using image_get_table_info_t = structures::table_info_t* ( __cdecl* )( const structures::image_t* image, int table_id );
		const auto image_get_table_info = reinterpret_cast < image_get_table_info_t >( GetProcAddress( mono_module, "mono_image_get_table_info" ) );

		using table_info_get_rows_t = std::int64_t ( __cdecl* )( const structures::table_info_t* table_info );
		const auto table_info_get_rows = reinterpret_cast < table_info_get_rows_t >( GetProcAddress( mono_module, "mono_table_info_get_rows" ) );

		using class_from_name_t = structures::class_t*( __cdecl* )( const structures::image_t*, const char*, const char* );
		const auto class_from_name = reinterpret_cast< class_from_name_t >( GetProcAddress( mono_module, "mono_class_from_name" ) );

		using class_get_method_from_name_t = structures::method_t* ( __cdecl* )( const structures::class_t*, const char*, int );
		const auto class_get_method_from_name = reinterpret_cast < class_get_method_from_name_t >( GetProcAddress( mono_module, "mono_class_get_method_from_name" ) );

		using class_get_fields_t = void* ( __cdecl* )( void*, void* );
		const auto class_get_fields = reinterpret_cast < class_get_fields_t >( GetProcAddress( mono_module, "mono_class_get_fields" ) );

		using class_get_t = structures::class_t*( __cdecl* )( const structures::image_t*, int32_t type_token );
		const auto class_get = reinterpret_cast <class_get_t>( GetProcAddress( mono_module, "mono_class_get" ) );

		using class_get_nested_types_t = structures::class_t*( __cdecl* )( const structures::class_t*, void** iter );
		const auto class_get_nested_types = reinterpret_cast <class_get_nested_types_t>( GetProcAddress( mono_module, "mono_class_get_nested_types" ) );

		using class_get_namespace_t = const char*( __cdecl* )( const structures::class_t* );
		const auto class_get_namespace = reinterpret_cast <class_get_namespace_t>( GetProcAddress( mono_module, "mono_class_get_namespace" ) );

		using class_get_name_t = const char*( __cdecl* )( const structures::class_t* );
		const auto class_get_name = reinterpret_cast <class_get_name_t>( GetProcAddress( mono_module, "mono_class_get_name" ) );

		using field_from_name_t = structures::field_t* ( __cdecl* ) ( const structures::class_t*, const char* );
		const auto field_from_name = reinterpret_cast < field_from_name_t >( GetProcAddress( mono_module, "mono_class_get_field_from_name" ) );

		using field_get_name_t = const char* ( __cdecl* )( std::uintptr_t );
		const auto field_get_name = reinterpret_cast < field_get_name_t >( GetProcAddress( mono_module, "mono_field_get_name" ) );

		using field_get_offset_t = std::uintptr_t ( __cdecl* )( const structures::field_t* );
		const auto field_get_offset = reinterpret_cast < field_get_offset_t >( GetProcAddress( mono_module, "mono_field_get_offset" ) );	

		using domain_assembly_open_t = structures::assembly_t* ( __cdecl* )( const structures::domain_t*, const char* );
		const auto domain_assembly_open = reinterpret_cast < domain_assembly_open_t >( GetProcAddress( mono_module, "mono_domain_assembly_open" ) );
	}

	namespace enums {
		enum e_meta_table {
			TABLE_MODULE,
			TABLE_TYPEREF,
			TABLE_TYPEDEF,
			TABLE_FIELD_POINTER,
			TABLE_FIELD,
			TABLE_METHOD_POINTER,
			TABLE_METHOD,
			TABLE_PARAM_POINTER,
			TABLE_PARAM,
			TABLE_INTERFACEIMPL,
			TABLE_MEMBERREF, /* 0xa */
			TABLE_CONSTANT,
			TABLE_CUSTOMATTRIBUTE,
			TABLE_FIELDMARSHAL,
			TABLE_DECLSECURITY,
			TABLE_CLASSLAYOUT,
			TABLE_FIELDLAYOUT, /* 0x10 */
			TABLE_STANDALONESIG,
			TABLE_EVENTMAP,
			TABLE_EVENT_POINTER,
			TABLE_EVENT,
			TABLE_PROPERTYMAP,
			TABLE_PROPERTY_POINTER,
			TABLE_PROPERTY,
			TABLE_METHODSEMANTICS,
			TABLE_METHODIMPL,
			TABLE_MODULEREF, /* 0x1a */
			TABLE_TYPESPEC,
			TABLE_IMPLMAP,
			TABLE_FIELDRVA,
			TABLE_UNUSED6,
			TABLE_UNUSED7,
			TABLE_ASSEMBLY, /* 0x20 */
			TABLE_ASSEMBLYPROCESSOR,
			TABLE_ASSEMBLYOS,
			TABLE_ASSEMBLYREF,
			TABLE_ASSEMBLYREFPROCESSOR,
			TABLE_ASSEMBLYREFOS,
			TABLE_FILE,
			TABLE_EXPORTEDTYPE,
			TABLE_MANIFESTRESOURCE,
			TABLE_NESTEDCLASS,
			TABLE_GENERICPARAM, /* 0x2a */
			TABLE_METHODSPEC,
			TABLE_GENERICPARAMCONSTRAINT
		};

		enum e_token_type {
			TOKEN_MODULE = 0x00000000,
			TOKEN_TYPE_REF = 0x01000000,
			TOKEN_TYPE_DEF = 0x02000000,
			TOKEN_FIELD_DEF = 0x04000000,
			TOKEN_METHOD_DEF = 0x06000000,
			TOKEN_PARAM_DEF = 0x08000000,
			TOKEN_INTERFACE_IMPL = 0x09000000,
			TOKEN_MEMBER_REF = 0x0a000000,
			TOKEN_CUSTOM_ATTRIBUTE = 0x0c000000,
			TOKEN_PERMISSION = 0x0e000000,
			TOKEN_SIGNATURE = 0x11000000,
			TOKEN_EVENT = 0x14000000,
			TOKEN_PROPERTY = 0x17000000,
			TOKEN_MODULE_REF = 0x1a000000,
			TOKEN_TYPE_SPEC = 0x1b000000,
			TOKEN_ASSEMBLY = 0x20000000,
			TOKEN_ASSEMBLY_REF = 0x23000000,
			TOKEN_FILE = 0x26000000,
			TOKEN_EXPORTED_TYPE = 0x27000000,
			TOKEN_MANIFEST_RESOURCE = 0x28000000,
			TOKEN_GENERIC_PARAM = 0x2a000000,
			TOKEN_METHOD_SPEC = 0x2b000000,

			/*
			* These do not match metadata tables directly
			*/
			TOKEN_STRING = 0x70000000,
			TOKEN_NAME = 0x71000000,
			TOKEN_BASE_TYPE = 0x72000000
		};
	}

	class mono_manager_t
	{
	public:
		
	};
}