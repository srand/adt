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

#ifndef ADT_SLIST_H
#define ADT_SLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/

typedef struct slist      slist_t;
typedef struct slist_link slist_link_t;
typedef void (*slist_traverser_t)(slist_t *list, void *elem, void *data);

struct slist_link
{
   slist_link_t *next;
};

struct slist
{
   slist_link_t  head;
   slist_link_t *tail;
};

/**************************************************************************/

void
slist_init(slist_t *list);

void
slist_link_init(slist_t *list, slist_link_t *link);

void
slist_insert_head(slist_t *list, slist_link_t *link);

void
slist_insert_tail(slist_t *list, slist_link_t *link);

void
slist_remove(slist_t *list, slist_link_t *link);

slist_link_t *
slist_head(slist_t *list);

slist_link_t *
slist_tail(slist_t *list);

slist_link_t *
slist_next(slist_t *list, slist_link_t *link);

void
slist_traverse_fwd(slist_t *list, slist_traverser_t func, void *udata);

/**************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ADT_SLIST_H */
