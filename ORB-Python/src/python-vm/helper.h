//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#ifndef MP_HELPER
#define MP_HELPER

#define ACCEPT_KW_ARG(Keyword, target, mp_type_getter) \
    if (args[Keyword].u_obj != MP_OBJ_NULL) {                    \
        target = mp_type_getter(args[Keyword].u_obj);                   \
    }

#define CONDITIONAL_CLAMP(condition, target, min_val, max_val) \
    ((condition) ? MIN(MAX((target), (min_val)), (max_val)) : (target)) \

//instance functions also get self as argument we want to ignore this for argument parsing
#define PARSE_KW_ARGS_INSTANCE_FUNCTION(n_args, pos_args, kw_args, allowed_args) \
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)]; \
    mp_arg_parse_all((n_args) - 1, (pos_args) + 1, (kw_args), MP_ARRAY_SIZE(allowed_args), (allowed_args), args);

#define PARSE_KW_ARGS_CONSTRUCTOR(n_args, pos_args, kw_args, allowed_args) \
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)]; \
    mp_arg_parse_all_kw_array((n_args), (n_kw), (all_args), MP_ARRAY_SIZE(allowed_args), (allowed_args), args);

#define ACCEPT_PORT(ARG, list) ({ \
    int port = args[ARG].u_int;\
    CHECK_VALID_PORT(port, list); \
    port; \
})

#define ACCEPT_ID(ARG, list) ({ \
    int id = args[ARG].u_int; \
    CHECK_VALID_ID(id, list); \
    id; \
})

#define CHECK_VALID_PORT(port, obj_list){\
    __CHECK_VALID(port, obj_list, "Invalid Port")\
}\

#define CHECK_VALID_ID(port, obj_list){\
    __CHECK_VALID(port, obj_list, "Invalid ID")\
}\

#define __CHECK_VALID(port, obj_list, text) \
    int port_max = sizeof(obj_list) / sizeof(obj_list[0]);\
    if ((port) >= (port_max) || (port) < 0) { \
        mp_raise_ValueError(MP_ERROR_TEXT(text)); \
    }

//this macro can be used if we expect floats or int
#define MP_OBJ_GET_NUMBER_VALUE(obj) (mp_obj_is_int(obj) ? (float)mp_obj_get_int(obj) : mp_obj_is_float(obj) ? mp_obj_get_float(obj) : (mp_raise_TypeError(MP_ERROR_TEXT("expected int or float")), 0))

#endif // MP_HELPER
