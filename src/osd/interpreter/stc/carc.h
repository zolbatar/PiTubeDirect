/* MIT License
 *
 * Copyright (c) 2022 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* carc: atomic reference counted shared_ptr
#include <stc/cstr.h>

typedef struct { cstr name, last; } Person;

Person Person_new(const char* name, const char* last) {
    return (Person){.name = cstr_from(name), .last = cstr_from(last)};
}
void Person_drop(Person* p) {
    printf("drop: %s %s\n", cstr_str(&p->name), cstr_str(&p->last));
    c_drop(cstr, &p->name, &p->last);
}

#define i_type ArcPers
#define i_key Person
#define i_keydrop Person_drop
#include <stc/carc.h>

int main() {
    ArcPers p = ArcPers_from(Person_new("John", "Smiths"));
    ArcPers q = ArcPers_clone(p); // share the pointer

    printf("%s %s. uses: %" PRIuMAX "\n", cstr_str(&q.get->name), cstr_str(&q.get->last), *q.use_count);
    c_drop(ArcPers, &p, &q);
}
*/
#include "ccommon.h"

#ifndef CARC_H_INCLUDED
#define CARC_H_INCLUDED
#include "forward.h"
#include <stdlib.h>

#if defined(__GNUC__) || defined(__clang__)
    typedef long catomic_long;
    #define c_atomic_inc(v) (void)__atomic_add_fetch(v, 1, __ATOMIC_SEQ_CST)
    #define c_atomic_dec_and_test(v) !__atomic_sub_fetch(v, 1, __ATOMIC_SEQ_CST)
#elif defined(_MSC_VER)
    #include <intrin.h>
    typedef long catomic_long;
    #define c_atomic_inc(v) (void)_InterlockedIncrement(v)
    #define c_atomic_dec_and_test(v) !_InterlockedDecrement(v)
#else
    #include <stdatomic.h>
    typedef _Atomic long catomic_long;
    #define c_atomic_inc(v) (void)atomic_fetch_add(v, 1)
    #define c_atomic_dec_and_test(v) (atomic_fetch_sub(v, 1) == 1)
#endif

#define carc_null {NULL, NULL}
#define _cx_carc_rep struct _cx_memb(_rep_)
#endif // CARC_H_INCLUDED

#ifndef _i_prefix
#define _i_prefix carc_
#endif
#if !(defined i_cmp || defined i_less || defined i_eq || defined i_hash)
  #define _i_no_cmp
#endif
#include "template.h"
typedef i_keyraw _cx_raw;

#if !c_option(c_no_atomic)
  #define _i_atomic_inc(v)          c_atomic_inc(v)
  #define _i_atomic_dec_and_test(v) c_atomic_dec_and_test(v)
#else
  #define _i_atomic_inc(v)          (void)(++*(v))
  #define _i_atomic_dec_and_test(v) !(--*(v))
#endif
#if !c_option(c_is_fwd)
_cx_deftypes(_c_carc_types, _cx_self, i_key);
#endif
_cx_carc_rep { catomic_long counter; i_key value; };

STC_INLINE _cx_self _cx_memb(_init)(void) 
    { return c_make(_cx_self){NULL, NULL}; }

STC_INLINE long _cx_memb(_use_count)(const _cx_self* self)
    { return self->use_count ? *self->use_count : 0; }

STC_INLINE _cx_self _cx_memb(_from_ptr)(_cx_value* p) {
    _cx_self ptr = {p};
    if (p) 
        *(ptr.use_count = c_alloc(catomic_long)) = 1;
    return ptr;
}

// c++: std::make_shared<_cx_value>(val)
STC_INLINE _cx_self _cx_memb(_from)(_cx_value val) {
    _cx_self ptr;
    _cx_carc_rep *rep = c_alloc(_cx_carc_rep);
    *(ptr.use_count = &rep->counter) = 1;
    *(ptr.get = &rep->value) = val;
    return ptr;
}

STC_INLINE _cx_self _cx_memb(_make)(_cx_value val) // [deprecated]
    { return _cx_memb(_from)(val); }

STC_INLINE _cx_raw _cx_memb(_toraw)(const _cx_self* self)
    { return i_keyto(self->get); }

STC_INLINE _cx_self _cx_memb(_move)(_cx_self* self) {
    _cx_self ptr = *self;
    self->get = NULL, self->use_count = NULL;
    return ptr;
}

STC_INLINE void _cx_memb(_drop)(_cx_self* self) {
    if (self->use_count && _i_atomic_dec_and_test(self->use_count)) {
        i_keydrop(self->get);
        if ((char *)self->get != (char *)self->use_count + offsetof(_cx_carc_rep, value))
            c_free(self->get);
        c_free((long*)self->use_count);
    }
}

STC_INLINE void _cx_memb(_reset)(_cx_self* self) {
    _cx_memb(_drop)(self);
    self->use_count = NULL, self->get = NULL;
}

STC_INLINE void _cx_memb(_reset_to)(_cx_self* self, _cx_value* p) {
    _cx_memb(_drop)(self);
    *self = _cx_memb(_from_ptr)(p);
}

#if !defined _i_no_emplace
    STC_INLINE _cx_self _cx_memb(_new)(_cx_raw raw)
        { return _cx_memb(_from)(i_keyfrom(raw)); }
#endif // !_i_no_emplace

// does not use i_keyclone, so OK to always define.
STC_INLINE _cx_self _cx_memb(_clone)(_cx_self ptr) {
    if (ptr.use_count)
        _i_atomic_inc(ptr.use_count);
    return ptr;
}

STC_INLINE void _cx_memb(_copy)(_cx_self* self, _cx_self ptr) {
    if (ptr.use_count)
        _i_atomic_inc(ptr.use_count);
    _cx_memb(_drop)(self);
    *self = ptr;
}

STC_INLINE void _cx_memb(_take)(_cx_self* self, _cx_self ptr) {
    if (self->get != ptr.get)
        _cx_memb(_drop)(self);
    *self = ptr;
}

STC_INLINE uint64_t _cx_memb(_value_hash)(const _cx_value* x) {
    #if defined _i_no_cmp
        return c_default_hash(&x);
    #else
        _cx_raw rx = i_keyto(x);
        return i_hash((&rx));
    #endif
}

STC_INLINE int _cx_memb(_value_cmp)(const _cx_value* x, const _cx_value* y) {
    #if defined _i_no_cmp
        return c_default_cmp(&x, &y);
    #else
        _cx_raw rx = i_keyto(x), ry = i_keyto(y);
        return i_cmp((&rx), (&ry));
    #endif
}

STC_INLINE bool _cx_memb(_value_eq)(const _cx_value* x, const _cx_value* y) {
    #if defined _i_no_cmp
        return x == y;
    #else
        _cx_raw rx = i_keyto(x), ry = i_keyto(y);
        return i_eq((&rx), (&ry));
    #endif
}

STC_INLINE uint64_t _cx_memb(_hash)(const _cx_self* x)
    { return _cx_memb(_value_hash)(x->get); }

STC_INLINE int _cx_memb(_cmp)(const _cx_self* x, const _cx_self* y)
    { return _cx_memb(_value_cmp)(x->get, y->get); }

STC_INLINE bool _cx_memb(_eq)(const _cx_self* x, const _cx_self* y)
    { return _cx_memb(_value_eq)(x->get, y->get); }

#undef _i_atomic_inc
#undef _i_atomic_dec_and_test
#include "template.h"
