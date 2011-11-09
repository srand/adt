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

#include "slist.h"
#include <stddef.h>

/**************************************************************************/

void
slist_init(slist_t *list)
{
   slist_link_init(list, &list->head);
   list->tail = &list->head;
}

/**************************************************************************/

void
slist_link_init(slist_t *list, slist_link_t *link)
{
   link->next = &list->head;
}

/**************************************************************************/

void
slist_insert_head(slist_t *list, slist_link_t *link)
{
   link->next = list->head.next;
   list->head.next = link;
}

/**************************************************************************/

void
slist_insert_tail(slist_t *list, slist_link_t *link)
{
   slist_link_init(list, link);
   list->tail->next = link;
   list->tail = link;
}

/**************************************************************************/

void
slist_remove(slist_t *list, slist_link_t *link)
{
   slist_link_t *prev = list->head.next;

   if (link == slist_head(list))
   {
      list->head.next = link->next;
   }
   else
   {
      slist_link_t *current = slist_next(list, prev);
      
      do {
         if (current == link)
         {
            prev->next = current->next;
            break;
         }

         current = slist_next(list, current);

      } while (current != NULL);
   }

   if (list->tail == link)
   {
      list->tail = list->head.next;
   }
}

/**************************************************************************/

slist_link_t *
slist_head(slist_t *list)
{
   return slist_next(list, &list->head);
}

/**************************************************************************/

slist_link_t *
slist_tail(slist_t *list)
{
   return list->tail != &list->head ? list->tail : NULL;
}

/**************************************************************************/

slist_link_t *
slist_next(slist_t *list, slist_link_t *link)
{
   return link->next != &list->head ? link->next : NULL;
}

/**************************************************************************/

void
slist_traverse_fwd(slist_t *list, slist_traverser_t func, void *udata)
{
   slist_link_t *link;

   for (link = slist_head(list);
        link != NULL;
        link = slist_next(list, link))
   {
      func(list, link, udata);
   }
}

/**************************************************************************/

