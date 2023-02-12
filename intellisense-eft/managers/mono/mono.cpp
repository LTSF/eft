#include "mono.hpp"

mono::structures::image_t* mono::structures::image_t::get_image_by_name( const char* const name )
{
	static const auto domain = functions::get_root_domain( );

	if ( const auto assembly = functions::domain_assembly_open( domain, name ) )
		return assembly->get_image( );

	return nullptr;
}

mono::structures::table_info_t* mono::structures::image_t::get_table_info(int table_id) const
{
	if (!this)
		return nullptr;
	return mono::functions::image_get_table_info(this, table_id);
}

const char* mono::structures::image_t::get_name( ) const
{
	if ( !this )
		return nullptr;

	return functions::image_get_name( this );
}

mono::structures::class_t* mono::structures::image_t::get_class( const char* const namespace_name, const char* const class_name ) const
{
	if ( !this )
		return nullptr;
	return functions::class_from_name(this, namespace_name, class_name);
}

std::vector< mono::structures::assembly_t* > mono::structures::assembly_t::get_assemblies( )
{
	std::vector< mono::structures::assembly_t* > assemblies;

	functions::assembly_foreach( 
		[ ] ( assembly_t* assembly, std::vector< mono::structures::assembly_t* >& assemblies )
		{
			if ( !assembly )
				return;

			assemblies.push_back( assembly );
		}, assemblies 
	);

	return assemblies;
}

mono::structures::image_t* mono::structures::assembly_t::get_image( ) const
{
	if ( !this )
		return nullptr;

	return functions::assembly_get_image( this );
}

mono::structures::class_t* mono::structures::class_t::get_nested_class(const char* class_name)
{
	if (!this)
		return nullptr;

	void* iter = NULL;
	while (true) {
		class_t* nested_class = functions::class_get_nested_types(this, &iter);
		if (!nested_class)
			break;

		if (!_strcmpi(class_name, nested_class->get_name()))
			return nested_class;
	}
	return 0;
}

mono::structures::method_t* mono::structures::class_t::get_method_from_name( const char* method_name, std::int32_t parameter_count ) const
{
	if ( !this )
		return nullptr;

	return functions::class_get_method_from_name( this, method_name, parameter_count );
}

mono::structures::field_t* mono::structures::class_t::get_field( const char* const name ) const
{
	if ( !this )
		return nullptr;

	return functions::field_from_name( this, name );
}

mono::structures::vtable_t* mono::structures::class_t::get_vtable() const
{
	if (!this)
		return nullptr;

	return functions::class_vtable(functions::get_root_domain(), this);
}

const char* mono::structures::class_t::get_namespace_name() const
{
	if (!this)
		return nullptr;
	return functions::class_get_namespace(this);
}

const char* mono::structures::class_t::get_name() const
{
	if (!this)
		return nullptr;
	return functions::class_get_name(this);
}

std::uintptr_t mono::structures::method_t::get_address( ) const
{
	if ( !this )
		return 0;

	return functions::compile_method( this );
}

std::uintptr_t mono::structures::field_t::get_offset( ) const
{
	if ( !this )
		return 0;

	return functions::field_get_offset( this );
}

std::uintptr_t mono::structures::vtable_t::get_static_field_data() const
{
	if (!this)
		return 0;

	return functions::vtable_get_static_field_data(this);
}

int64_t mono::structures::table_info_t::get_rows() const
{
	if (!this)
		return 0;

	return functions::table_info_get_rows(this);
}

mono::structures::class_t* mono::structures::object_t::get_class() const
{
	if (!this)
		return 0;

	return functions::object_get_class(this);
}
