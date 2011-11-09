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

#include "list.h"
#include <stddef.h>

/**************************************************************************/

void
list_init(list_t *list)
{
   list_link_init(list, list);
}

/**************************************************************************/

void
list_link_init(list_t *list, list_link_t *link)
{
   link->next = list;
   link->prev = list;
}

/**************************************************************************/

int
list_link_detached(list_t *list, list_link_t *link)
{
   (void)list;
   return link->next == NULL || link->prev == NULL;
}

/**************************************************************************/

void
list_link_detach(list_t *list, list_link_t *link)
{
   (void)list;
   link->next = NULL;
   link->prev = NULL;
}

/**************************************************************************/

void
list_insert_head(list_t *list, list_link_t *link)
{
   list_link_init(list, link);

   link->next = list->next;
   link->next->prev = link;
   list->next = link;
}

/**************************************************************************/

void
list_insert_tail(list_t *list, list_link_t *link)
{
   list_link_init(list, link);

   link->prev = list->prev;
   link->prev->next = link;
   list->prev = link;
}

/**************************************************************************/

void
list_insert(list_t *list, 
            list_link_t *after,
            list_link_t *link)
{
   list_link_init(list, link);

   link->prev = after;
   link->next = after->next;
   link->prev->next = link;
   link->next->prev = link;
}

/**************************************************************************/

void
list_remove(list_t *list, list_link_t *link)
{
   (void)list;
   link->next->prev = link->prev;
   link->prev->next = link->next;
   link->next = NULL;
   link->prev = NULL;
}

/**************************************************************************/

list_link_t *
list_next(list_t *list, list_link_t *link)
{
   return (link->next != list) ? link->next : NULL;
}

/**************************************************************************/

list_link_t *
list_prev(list_t *list, list_link_t *link)
{
   return (link->prev != list) ? link->prev : NULL;
}

/**************************************************************************/

list_link_t *
list_head(list_t *list)
{
   return list_next(list, list);
}

/**************************************************************************/

list_link_t *
list_tail(list_t *list)
{
   return list_prev(list, list);
}

/**************************************************************************/

void
list_traverse_fwd(list_t *list, list_traverser_t func, void *user_data)
{
   list_link_t *link;

   for (link = list_head(list);
        link != NULL;
        link = list_next(list, link))
   {
      func(list, link, user_data);
   }
}

/**************************************************************************/

void
list_traverse_bwd(list_t *list, list_traverser_t func, void *user_data)
{
   list_link_t *link;

   for (link = list_tail(list);
        link != NULL;
        link = list_prev(list, link))
   {
      func(list, link, user_data);
   }
}

/**************************************************************************/
