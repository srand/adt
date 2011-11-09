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

#ifndef ADT_RBTREE_H
#define ADT_RBTREE_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/

#define RBTREE_BLACK  0ul
#define RBTREE_RED    1ul

/**************************************************************************/

typedef unsigned long      rbtree_color_t;
typedef struct rbtree      rbtree_t;
typedef struct rbtree_node rbtree_node_t;
typedef int                (*rbtree_comparer_t)(void *a, void *b);

struct rbtree_node
{
   rbtree_node_t  *left;
   rbtree_node_t  *right;
   rbtree_node_t  *parent;
   rbtree_color_t  color;
};

struct rbtree
{
   rbtree_node_t     *root;
   rbtree_node_t      nil;
   rbtree_comparer_t  compare;
};

/**************************************************************************/

void
rbtree_init(rbtree_t *tree, rbtree_comparer_t compare);

int
rbtree_insert(rbtree_t *tree, rbtree_node_t *node);

void
rbtree_remove(rbtree_t *tree, rbtree_node_t *node);

rbtree_node_t *
rbtree_find(rbtree_t *tree, rbtree_comparer_t compare, void *key);

rbtree_node_t *
rbtree_find_best(rbtree_t *tree, rbtree_comparer_t compare, void *key);

rbtree_node_t *
rbtree_first(rbtree_t *tree);

rbtree_node_t *
rbtree_next(rbtree_t *tree, rbtree_node_t *node);

/**************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ADT_RBTREE_H */