/* SNCL typeid vX.XX
   Defines helpful type information stuff for c versions

   Contributors:
   - StarIitNova (fynotix.dev@gmail.com)
 */

#ifndef SNCL_TYPEID_H__
#define SNCL_TYPEID_H__

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#if defined(__GNUC__) || defined(__clang__)
#define typeof __typeof__
#else
#error "typeof is not supported by this compiler in pre-C23 mode"
#endif
#endif

typedef void *iterator_t;

#endif // SNCL_TYPEID_H__
