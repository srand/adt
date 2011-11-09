/* COPYRIGHT-BEGIN-R1
 ***************************************************************************
 * Copyright 2011 Robert Andersson. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 * 
 * THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of the copyright holder.
 ***************************************************************************
 * COPYRIGHT-END */

#ifndef ADT_HASH_H
#define ADT_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"
#include <stddef.h>

/**************************************************************************/

typedef list_t        hash_table_t;
typedef list_link_t   hash_link_t;
typedef unsigned long hash_key_t; 
typedef struct hash   hash_t;
typedef hash_key_t    (*hash_func_t)(hash_t *hash, hash_link_t *element);

struct hash
{
   hash_table_t  *hash_table;
   hash_func_t    hash_func;
   size_t         hash_size;
};

/**************************************************************************/

void
hash_init(hash_t *hash, hash_table_t *table, hash_func_t func, size_t size);

hash_key_t
hash_key(hash_t *hash, hash_link_t *element);

int
hash_insert(hash_t *hash, hash_link_t *element);

int
hash_remove(hash_t *hash, hash_link_t *element);

hash_link_t *
hash_find(hash_t *hash, hash_key_t key);

/**************************************************************************/
   
#ifdef __cplusplus
}
#endif

#endif /* ADT_HASH_H */


