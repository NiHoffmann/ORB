#ifndef MP_HELPER
#define MP_HELPER

#define ACCEPT_VALUE_KW_GIVEN(Keyword, target, mp_type_getter) \
    if (args[Keyword].u_obj != MP_OBJ_NULL) {                    \
        target = mp_type_getter(args[Keyword].u_obj);                   \
    }

#define ACCEPT_VALUES_NO_KW_2(n_args, Keyword1, Target1, type_getter1, Keyword2, Target2, type_getter2) \
    switch(n_args) { \
        case 1: \
            Target1 = (type_getter1)(args[Keyword1].u_obj); \
            break; \
        case 2: \
            Target1 = (type_getter1)(args[Keyword1].u_obj); \
            Target2 = (type_getter2)(args[Keyword2].u_obj); \
            break; \
        default: \
            break; \
    }


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

//TODO make this also use a static list of sensors, maybe move static sensor list to its own file so there will be no duplicate elements
#define GENERATE_SIMPLE_SENSOR(name, _get_func, _conf_func) \
    \
    typedef struct _##name##_obj_t { \
        mp_obj_base_t base; \
        uint8_t port; \
    } name##_obj_t; \
    \
    static void mp_##name##_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) { \
        const name##_obj_t *self = MP_OBJ_TO_PTR(self_in); \
        mp_printf(print, #name "(%d)", self->port); \
    } \
    \
    static mp_obj_t mp_##name##_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) { \
        enum { ARG_port }; \
        static const mp_arg_t allowed_args[] = { \
            { MP_QSTR_port, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1} }, \
        }; \
        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)]; \
        mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args); \
        \
        int port = args[ARG_port].u_int; \
        if ((port) >= (4) || (port) < 0) { \
        mp_raise_ValueError(MP_ERROR_TEXT("Invalid Port")); \
        }\
        name##_obj_t *self = m_new_obj(name##_obj_t); \
        _conf_func(port);\
        self->base.type = type; \
        self->port = port; \
        \
        return MP_OBJ_FROM_PTR(self); \
    } \
    \
    static mp_obj_t get_func(mp_obj_t self_in) { \
        name##_obj_t *self = MP_OBJ_TO_PTR(self_in); \
        sensor_return_values *ret = getSensor(self->port); \
        return _get_func(ret);  \
    } \
    static MP_DEFINE_CONST_FUN_OBJ_1(get_func##_obj, get_func); \
    \
    static const mp_rom_map_elem_t name##_local_dict_table[] = { \
        { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&get_func##_obj) }, \
    }; \
    static MP_DEFINE_CONST_DICT(name##_local, name##_local_dict_table); \
    \
    MP_DEFINE_CONST_OBJ_TYPE( \
        name##_type, \
        MP_QSTR_##name, \
        MP_TYPE_FLAG_NONE, \
        make_new, mp_##name##_make_new, \
        print, mp_##name##_print, \
        locals_dict, &name##_local \
    );

#endif // MP_HELPER
