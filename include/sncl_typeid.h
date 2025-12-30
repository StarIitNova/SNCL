#ifndef SNCL_TYPEID_H__
#define SNCL_TYPEID_H__

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#if defined(__GNUC__) || defined(__clang__)
#define typeof __typeof__
#else
#error "typeof is not supported by this compiler in pre-C23 mode"
#endif
#endif

#endif // SNCL_TYPEID_H__
