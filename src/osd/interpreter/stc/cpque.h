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
#include "ccommon.h"

#ifndef CPQUE_H_INCLUDED
#include <stdlib.h>
#include "forward.h"
#endif

#ifndef _i_prefix
#define _i_prefix cpque_
#endif

#include "template.h"

#if !c_option(c_is_fwd)
   _cx_deftypes(_c_cpque_types, _cx_self, i_key);
#endif
typedef i_keyraw _cx_raw;

STC_API void _cx_memb(_make_heap)(_cx_self* self);
STC_API void _cx_memb(_erase_at)(_cx_self* self, size_t idx);
STC_API void _cx_memb(_push)(_cx_self* self, _cx_value value);

STC_INLINE _cx_self _cx_memb(_init)(void)
    { return c_make(_cx_self){NULL}; }

STC_INLINE bool _cx_memb(_reserve)(_cx_self* self, const size_t cap) {
    if (cap != self->_len && cap <= self->_cap) return true;
    _cx_value *d = (_cx_value *)c_realloc(self->data, cap*sizeof *d);
    return d ? (self->data = d, self->_cap = cap, true) : false;
}

STC_INLINE void _cx_memb(_shrink_to_fit)(_cx_self* self)
    { _cx_memb(_reserve)(self, self->_len); }

STC_INLINE _cx_self _cx_memb(_with_capacity)(const size_t cap) {
    _cx_self out = {NULL}; _cx_memb(_reserve)(&out, cap);
    return out;
}

STC_INLINE _cx_self _cx_memb(_with_size)(const size_t size, i_key null) {
    _cx_self out = {NULL}; _cx_memb(_reserve)(&out, size);
    while (out._len < size) out.data[out._len++] = null;
    return out;
}

STC_INLINE void _cx_memb(_clear)(_cx_self* self) {
    size_t i = self->_len; self->_len = 0;
    while (i--) { i_keydrop((self->data + i)); }
}

STC_INLINE void _cx_memb(_drop)(_cx_self* self)
    { _cx_memb(_clear)(self); c_free(self->data); }

STC_INLINE size_t _cx_memb(_size)(const _cx_self* q)
    { return q->_len; }

STC_INLINE bool _cx_memb(_empty)(const _cx_self* q)
    { return !q->_len; }

STC_INLINE size_t _cx_memb(_capacity)(const _cx_self* q)
    { return q->_cap; }

STC_INLINE const _cx_value* _cx_memb(_top)(const _cx_self* self)
    { return &self->data[0]; }

STC_INLINE void _cx_memb(_pop)(_cx_self* self)
    { assert(!_cx_memb(_empty)(self)); _cx_memb(_erase_at)(self, 0); }

#if !defined _i_no_clone
STC_API _cx_self _cx_memb(_clone)(_cx_self q);

STC_INLINE void _cx_memb(_copy)(_cx_self *self, const _cx_self* other) {
    if (self->data == other->data) return;
    _cx_memb(_drop)(self);
    *self = _cx_memb(_clone)(*other);
}
STC_INLINE i_key _cx_memb(_value_clone)(_cx_value val)
    { return i_keyclone(val); }
#endif // !_i_no_clone

#if !defined _i_no_emplace
STC_INLINE void _cx_memb(_emplace)(_cx_self* self, _cx_raw raw)
    { _cx_memb(_push)(self, i_keyfrom(raw)); }
#endif // !_i_no_emplace

/* -------------------------- IMPLEMENTATION ------------------------- */
#if defined(i_implement)

STC_DEF void
_cx_memb(_sift_down_)(_cx_value* arr, const size_t idx, const size_t n) {
    for (size_t r = idx, c = idx*2; c <= n; c *= 2) {
        c += (c < n && (i_less((&arr[c]), (&arr[c + 1]))));
        if (!(i_less((&arr[r]), (&arr[c])))) return;
        _cx_value t = arr[r]; arr[r] = arr[c]; arr[r = c] = t;
    }
}

STC_DEF void
_cx_memb(_make_heap)(_cx_self* self) {
    size_t n = self->_len;
    _cx_value *arr = self->data - 1;
    for (size_t k = n/2; k != 0; --k)
        _cx_memb(_sift_down_)(arr, k, n);
}

#if !defined _i_no_clone
STC_DEF _cx_self _cx_memb(_clone)(_cx_self q) {
    _cx_self out = _cx_memb(_with_capacity)(q._len);
    for (; out._len < out._cap; ++q.data)
        out.data[out._len++] = i_keyclone((*q.data));
    return out;
}
#endif

STC_DEF void
_cx_memb(_erase_at)(_cx_self* self, const size_t idx) {
    i_keydrop((self->data + idx));
    const size_t n = --self->_len;
    self->data[idx] = self->data[n];
    _cx_memb(_sift_down_)(self->data - 1, idx + 1, n);
}

STC_DEF void
_cx_memb(_push)(_cx_self* self, _cx_value value) {
    if (self->_len == self->_cap)
        _cx_memb(_reserve)(self, self->_len*3/2 + 4);
    _cx_value *arr = self->data - 1; /* base 1 */
    size_t c = ++self->_len;
    for (; c > 1 && (i_less((&arr[c/2]), (&value))); c /= 2)
        arr[c] = arr[c/2];
    arr[c] = value;
}

#endif
#define CPQUE_H_INCLUDED
#include "template.h"
