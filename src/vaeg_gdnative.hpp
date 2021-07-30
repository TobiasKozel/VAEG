/**
 * Some boilerplate code for gdnative
 */

#ifndef VAEG_GDNATIVE
#define VAEG_GDNATIVE

#include <new>
#include <gdnative_api_struct.gen.h>

namespace vaeg {
	const godot_gdnative_core_api_struct *api = NULL;
	const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

	/**
	 * called to register all classes and their methods
	 */
	void init_gdnative(void* handle);
}

#define VAEG_ERROR(message) 													\
vaeg::api->godot_print_error(message, __PRETTY_FUNCTION__, __FILE__, __LINE__);

#define VAEG_PRINT(message) 								\
{															\
	godot_string string;									\
	vaeg::api->godot_string_new(&string);					\
	vaeg::api->godot_string_parse_utf8(&string, message);	\
	vaeg::api->godot_print(&string);						\
	vaeg::api->godot_string_destroy(&string);				\
}

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	vaeg::api = p_options->api_struct;
	for (int i = 0; i < vaeg::api->num_extensions; i++) {
		switch (vaeg::api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				vaeg::nativescript_api =
				(godot_gdnative_ext_nativescript_api_struct *) vaeg::api->extensions[i];
			}; break;
			default: break;
		};
	};
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	vaeg::api = NULL;
	vaeg::nativescript_api = NULL;
}

/**
 * This is a exact copy of the types defined in variant.h from godot
 * but lower cased so it can be abused with macro concats.
 */
enum VAEG_TYPE {
	godot_variant_type_nil,

	// atomic types
	godot_variant_type_bool,
	godot_variant_type_int,
	godot_variant_type_real,
	godot_variant_type_string,

	// math types

	godot_variant_type_vector2, // 5
	godot_variant_type_rect2,
	godot_variant_type_vector3,
	godot_variant_type_transform2d,
	godot_variant_type_plane,
	godot_variant_type_quat, // 10
	godot_variant_type_aabb,
	godot_variant_type_basis,
	godot_variant_type_transform,

	// misc types
	godot_variant_type_color,
	godot_variant_type_node_path, // 15
	godot_variant_type_rid,
	godot_variant_type_object,
	godot_variant_type_dictionary,
	godot_variant_type_array, // 20

	// arrays
	godot_variant_type_pool_byte_array,
	godot_variant_type_pool_int_array,
	godot_variant_type_pool_real_array,
	godot_variant_type_pool_string_array,
	godot_variant_type_pool_vector2_array, // 25
	godot_variant_type_pool_vector3_array,
	godot_variant_type_pool_color_array,
};

/**
 * Add Static constructor destrouctor using the godot api
 */
#define VAEG_CLASS(name)																					\
	godot_object* instance = nullptr;																		\
public:																										\
	/** Generated Constructor */																			\
	static GDCALLINGCONV void* _construct(godot_object* p_instance, void* p_method_data) {					\
		void* ptr = vaeg::api->godot_alloc(sizeof(name));													\
		name* obj = new (ptr) name();																		\
		obj->instance = p_instance;																			\
		return ptr;																							\
	}																										\
	/** Generated destructor */																				\
	static GDCALLINGCONV void _destroy(godot_object *p_instance, void *p_method_data, void *p_user_data) {	\
		name* instance = reinterpret_cast<name*>(p_user_data);												\
		instance->~name();																					\
		vaeg::api->godot_free(p_user_data);																	\
	}

/**
 * Adds getter and setter functions
 */
#define VAEG_SETGET(p_name, p_prop, p_type)									\
public:																		\
	/** Generated setter */													\
	static void _set_ ##p_prop(												\
		godot_object* instance_pointer, void* method_data,					\
		void* user_data, godot_variant* value								\
	) {																		\
		auto instance = reinterpret_cast<p_name*>(user_data);				\
		instance->p_prop = vaeg::api->godot_variant_as_ ## p_type (value);	\
	}																		\
	/** Generated getter */													\
	static godot_variant _get_ ##p_prop(									\
		godot_object* instance_pointer, void* method_data, void* user_data	\
	) {																		\
		auto instance = reinterpret_cast<p_name*>(user_data);				\
		godot_variant ret;													\
		vaeg::api->godot_variant_new_ ## p_type (&ret, instance->p_prop);	\
		return ret;															\
	}

/**
 * Register class to godot api
 */
#define VAEG_REGISTER_CLASS(p_name, p_base)					\
vaeg::nativescript_api->godot_nativescript_register_class(	\
	handle, #p_name, #p_base,								\
	{ &p_name::_construct, NULL, NULL },					\
	{ &p_name::_destroy, NULL, NULL }						\
);															\
p_name::_register(handle);


/**
 * Register a static function on a class
 */
#define VAEG_REGISTER_METHOD(handle, name, function)			\
{																\
	godot_instance_method name ## _ ## function = { };			\
	name ## _ ## function.method = &name::function;				\
	vaeg::nativescript_api->godot_nativescript_register_method(	\
		handle, #name, #function,								\
		{ GODOT_METHOD_RPC_MODE_DISABLED },						\
		name ## _ ## function									\
	);															\
}

#define VAEG_REGISTER_PROP(handle, p_name, p_prop, p_type, p_init) \
VAEG_REGISTER_PROP_HINT(											\
	handle, p_name, p_prop, p_type, p_init,							\
	GODOT_PROPERTY_HINT_NONE, ""									\
)
/**
 * Register a property of a class
 * Needs to have setters and getters
 */
#define VAEG_REGISTER_PROP_HINT(handle, p_name, p_prop, p_type, p_init, p_hint_t, p_hint) 	\
{																							\
	godot_property_attributes attr = { };													\
	attr.rset_type = GODOT_METHOD_RPC_MODE_DISABLED;										\
	attr.usage = GODOT_PROPERTY_USAGE_DEFAULT;												\
	attr.type = VAEG_TYPE::godot_variant_type_ ## p_type;									\
	attr.hint = p_hint_t;																	\
	godot_string hint;																		\
	vaeg::api->godot_string_new(&hint);														\
	vaeg::api->godot_string_parse_utf8(&hint, p_hint);										\
	attr.hint_string = hint;																\
	godot_variant init;																		\
	vaeg::api->godot_variant_new_ ## p_type (&init, p_init);								\
	godot_property_set_func set_func = { };													\
	set_func.method_data = NULL;															\
	set_func.free_func = NULL;																\
	set_func.set_func = p_name::_set_ ##p_prop;												\
	godot_property_get_func get_func = { };													\
	get_func.method_data = NULL;															\
	get_func.free_func = NULL;																\
	get_func.get_func = p_name::_get_ ##p_prop;												\
	vaeg::nativescript_api->godot_nativescript_register_property(							\
		handle, #p_name, #p_prop, &attr, set_func, get_func									\
	);																						\
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
	vaeg::init_gdnative(handle);
	VAEG_PRINT("Register VAEG")
}

#endif // VAEG_GDNATIVE
