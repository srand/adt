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

#include "hash.h"

/**************************************************************************/

#define false 0
#define true  1

/**************************************************************************/

void
hash_init(hash_t *hash, hash_table_t *table, hash_func_t func, size_t size)
{
   unsigned long index;
   
   hash->hash_table = table;
   hash->hash_func  = func;
   hash->hash_size  = size;
   
   for (index = 0; index < size; index++)
   {
      list_init(&table[index]);
   }
}

/**************************************************************************/

hash_key_t
hash_key(hash_t *hash, hash_link_t *element)
{
   return hash->hash_func(hash, element);
}

/**************************************************************************/

int
hash_insert(hash_t *hash, hash_link_t *element)
{
   hash_key_t key;
   
   key = hash->hash_func(hash, element);
   key = key & (hash->hash_size-1);
   
   list_insert_head(&hash->hash_table[key], element);

   return true;
}

/**************************************************************************/

int
hash_remove(hash_t *hash, hash_link_t *element)
{
   hash_link_t *link;
   hash_key_t   key;
   hash_key_t   key_index;
   
   key = hash->hash_func(hash, element);
   key_index = key & (hash->hash_size-1);
   
   for (link = list_head(&hash->hash_table[key_index]);
        link != NULL;
        link = list_next(&hash->hash_table[key_index], link))
   {
      if (key == hash->hash_func(hash, link))
      {
         list_remove(&hash->hash_table[key_index], element);
         return true;
      }
   }
   
   return false;
}

/**************************************************************************/

hash_link_t *
hash_find(hash_t *hash, hash_key_t key)
{
   hash_link_t *link;
   hash_key_t   key_index;
   
   key_index = key & (hash->hash_size-1);
   
   for (link = list_head(&hash->hash_table[key_index]);
        link != NULL;
        link = list_next(&hash->hash_table[key_index], link))
   {
      if (key == hash->hash_func(hash, link))
         return link;
   }
   
   return NULL;
}

/**************************************************************************/

