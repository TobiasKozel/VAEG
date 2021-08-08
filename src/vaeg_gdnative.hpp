/**
 * Some boilerplate code for gdnative
 * Absolute macro madness which in no way resemples
 * good practice but is easy to write
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

#define VAEG_PRINT(message) {								\
	godot_string string;									\
	vaeg::api->godot_string_new(&string);					\
	vaeg::api->godot_string_parse_utf8(&string, message);	\
	vaeg::api->godot_print(&string);						\
	vaeg::api->godot_string_destroy(&string);				\
}

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options* options) {
	vaeg::api = options->api_struct;

	for (int i = 0; i < vaeg::api->num_extensions; i++) {
		switch (vaeg::api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				vaeg::nativescript_api =
				(godot_gdnative_ext_nativescript_api_struct *) vaeg::api->extensions[i];
			}; break;
			default: break;
		};
	};
	VAEG_PRINT("Init VAEG")
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	vaeg::api = NULL;
	vaeg::nativescript_api = NULL;
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
	vaeg::init_gdnative(handle);
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
 *
 * 			Macros to register a class and its properties/function
 *
 */


/**
 * Add Static constructor destrouctor using the godot api
 * and a member variable with a pointer to the godot_object which the script is attached to
 */
#define VAEG_REGISTER_CLASS(p_class, p_base)																\
	godot_object* instance = nullptr;																		\
	using VAEG_CLASS_TYPE = p_class;																		\
	static constexpr char* VAEG_NAME = #p_class;															\
public:																										\
	/** Generated Constructor */																			\
	static GDCALLINGCONV void* _construct(godot_object* p_instance, void* p_method_data) {					\
		void* ptr = vaeg::api->godot_alloc(sizeof(VAEG_CLASS_TYPE));										\
		VAEG_CLASS_TYPE* obj = new (ptr) VAEG_CLASS_TYPE();													\
		obj->instance = p_instance;																			\
		return ptr;																							\
	}																										\
	/** Generated destructor */																				\
	static GDCALLINGCONV void _destroy(godot_object *p_instance, void *p_method_data, void *p_user_data) {	\
		VAEG_CLASS_TYPE* instance = reinterpret_cast<VAEG_CLASS_TYPE*>(p_user_data);						\
		instance->~VAEG_CLASS_TYPE();																		\
		vaeg::api->godot_free(p_user_data);																	\
	}																										\
	static void __register(void* handle) {																	\
		vaeg::nativescript_api->godot_nativescript_register_class(											\
			handle, VAEG_NAME, #p_base,																		\
			{ &VAEG_CLASS_TYPE::_construct, NULL, NULL },													\
			{ &VAEG_CLASS_TYPE::_destroy, NULL, NULL }														\
		);																									\
		__register(handle);																					\
	}																										\
	static void _register(void* handle)


/**
 * Register a function on a class
 * used in _register function of class
 */
#define VAEG_REGISTER_METHOD(p_name)							\
{																\
	godot_instance_method _ ## p_name = { };					\
	_ ## p_name.method = &VAEG_CLASS_TYPE:: ## _ ## p_name;		\
	vaeg::nativescript_api->godot_nativescript_register_method(	\
		handle, VAEG_NAME, #p_name,								\
		{ GODOT_METHOD_RPC_MODE_DISABLED },						\
		_ ## p_name												\
	);															\
}

/**
 * Register a property on a class
* Needs to have setters and getters generated with VAEG_SETGET
 * used in _register function of class
 */
#define VAEG_REGISTER_PROP(p_prop, p_type, p_init)	\
VAEG_REGISTER_PROP_HINT(							\
	p_prop, p_type, p_init,							\
	GODOT_PROPERTY_HINT_NONE, ""					\
)

/**
 * Register a property of a class with type hint for the inspector
 * Needs to have setters and getters generated with VAEG_SETGET
 * used in _register function of class
 */
#define VAEG_REGISTER_PROP_HINT(p_prop, p_type, p_init, p_hint_t, p_hint) 			\
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
	set_func.set_func = VAEG_CLASS_TYPE::_set_ ##p_prop;									\
	godot_property_get_func get_func = { };													\
	get_func.method_data = NULL;															\
	get_func.free_func = NULL;																\
	get_func.get_func = VAEG_CLASS_TYPE::_get_ ##p_prop;									\
	vaeg::nativescript_api->godot_nativescript_register_property(							\
		handle, VAEG_NAME, #p_prop, &attr, set_func, get_func								\
	);																						\
	vaeg::api->godot_string_destroy(&hint);													\
}

/**
 *
 * 				Macros to generate function heads and setters/getters
 *
 */


/**
 * Generates generic setter and getter for a property to a class
 */
#define VAEG_SETGET(p_prop, p_type) VAEG_SETGET_LOC(p_prop, p_type, p_prop)

/**
 * Generates generic setter and getter for a property to a class
 */
#define VAEG_SETGET_LOC(p_prop, p_type, p_location)								\
public:																			\
	/** Generated setter */														\
	static void _set_ ##p_prop(													\
		godot_object* instance_pointer, void* method_data,						\
		void* user_data, godot_variant* value									\
	) {																			\
		auto instance = reinterpret_cast<VAEG_CLASS_TYPE*>(user_data);			\
		instance->p_location = vaeg::api->godot_variant_as_ ## p_type (value);	\
	}																			\
	/** Generated getter */														\
	static godot_variant _get_ ##p_prop(										\
		godot_object* instance_pointer, void* method_data, void* user_data		\
	) {																			\
		auto instance = reinterpret_cast<VAEG_CLASS_TYPE*>(user_data);			\
		godot_variant ret;														\
		vaeg::api->godot_variant_new_ ## p_type (&ret, instance->p_location);	\
		return ret;																\
	}

/**
 * Generates function head to a godot setter function and
 * casts the parameter to the correct type
 */
#define VAEG_SET(p_prop, p_typ)												\
public:																		\
	static void _set ## _ ## p_prop ## (									\
		godot_object*, void*, void* user_data, godot_variant* value			\
	) {																		\
		auto instance = reinterpret_cast<VAEG_CLASS_TYPE*>(user_data);		\
		auto obj = vaeg::api->godot_variant_as_ ## p_typ ## (value);		\
		instance->set ## _ ## p_prop ## (obj);								\
	}																		\
	void set ## _ ## p_prop ##(godot_ ## p_typ ## * parameter)

/**
 * Generates a function head to retrieve a value
 */
#define VAEG_GET(p_prop)											\
public:																		\
	static godot_variant _get ## _ ## p_prop ##(							\
		godot_object*, void*, void* user_data								\
	) {																		\
		auto instance = reinterpret_cast<VAEG_CLASS_TYPE*>(user_data);		\
		return instance->set ## _ ## p_prop ##();							\
	}																		\
	godot_variant set ## _ ## p_prop ##()

/**
 * Generate function head and wrap static call to object
 * used in class
 */
#define VAEG_FUNC(p_name)															\
public:																				\
	static godot_variant _ ## p_name ## (											\
		godot_object*, void*, void* user_data, int num_args, godot_variant** args	\
	) {																				\
		auto obj = reinterpret_cast<VAEG_CLASS_TYPE*>(user_data);					\
		return obj->p_name(num_args, args);											\
	}																				\
	godot_variant p_name(int num_args, godot_variant** args)


#endif // VAEG_GDNATIVE
