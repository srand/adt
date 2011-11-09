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

#ifndef ADT_LIST_H
#define ADT_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/

struct list_link
{
   struct list_link *next;
   struct list_link *prev;
};

typedef struct list_link list_link_t;
typedef struct list_link list_t;
typedef void (*list_traverser_t)(list_t *list, void *elem, void *data);

/**************************************************************************/

void
list_init(list_t *list);

void
list_link_init(list_t *list, list_link_t *link);

void
list_link_detach(list_t *list, list_link_t *link);

int
list_link_detached(list_t *list, list_link_t *link);

void
list_insert_head(list_t *list, list_link_t *link);

void
list_insert_tail(list_t *list, list_link_t *link);

void
list_insert(list_t *list, list_link_t *after, list_link_t *link);

void
list_remove(list_t *list, list_link_t *link);

list_link_t *
list_next(list_t *list, list_link_t *link);

list_link_t *
list_prev(list_t *list, list_link_t *link);

list_link_t *
list_head(list_t *list);

list_link_t *
list_tail(list_t *list);

void
list_traverse_fwd(list_t *list, list_traverser_t func, void *user_data);

void
list_traverse_bwd(list_t *list, list_traverser_t func, void *user_data);

/**************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ADT_LIST_H */
