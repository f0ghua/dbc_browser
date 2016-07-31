
#ifndef VECTOR_DBC_EXPORT_H
#define VECTOR_DBC_EXPORT_H

#ifdef VECTOR_DBC_STATIC_DEFINE
#  define VECTOR_DBC_EXPORT
#  define VECTOR_DBC_NO_EXPORT
#else
#  ifndef VECTOR_DBC_EXPORT
#    ifdef Vector_DBC_EXPORTS
        /* We are building this library */
#      define VECTOR_DBC_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VECTOR_DBC_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VECTOR_DBC_NO_EXPORT
#    define VECTOR_DBC_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VECTOR_DBC_DEPRECATED
#  define VECTOR_DBC_DEPRECATED __attribute__ ((__deprecated__))
#  define VECTOR_DBC_DEPRECATED_EXPORT VECTOR_DBC_EXPORT __attribute__ ((__deprecated__))
#  define VECTOR_DBC_DEPRECATED_NO_EXPORT VECTOR_DBC_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VECTOR_DBC_NO_DEPRECATED
#endif

#endif
