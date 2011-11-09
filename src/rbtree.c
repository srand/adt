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

#include "rbtree.h"
#include <stddef.h>

/**************************************************************************/

#define rbtree_left(x)        ((x)->left)
#define rbtree_right(x)       ((x)->right)
#define rbtree_parent(x)      ((x)->parent)
#define rbtree_root(x)        ((x)->root)
#define rbtree_black(x)       ((x)->color == RBTREE_BLACK)
#define rbtree_red(x)         ((x)->color == RBTREE_RED)
#define rbtree_color(x)       ((x)->color)
#define rbtree_nil(x)         (&(x)->nil)
#define rbtree_left_child(x)  ((x) == rbtree_left(rbtree_parent(x)))
#define rbtree_right_child(x) ((x) == rbtree_right(rbtree_parent(x)))
#define rbtree_compare(tree)  ((tree)->compare)

#define false 0
#define true  1

/**************************************************************************/

static void
rbtree_node_init(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_left(node)   = rbtree_nil(tree);
   rbtree_right(node)  = rbtree_nil(tree);
   rbtree_parent(node) = rbtree_nil(tree);
   rbtree_color(node)  = RBTREE_BLACK;
}

/**************************************************************************/

static void 
rbtree_rotate_left(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_node_t *right  = rbtree_right(node);
   rbtree_node_t *parent = rbtree_parent(node);

   rbtree_right(node) = rbtree_left(right);
   
   if (rbtree_right(node) != rbtree_nil(tree))
   {
      rbtree_parent(rbtree_left(right)) = node;
   }
   
   rbtree_left(right)   = node;
   rbtree_parent(right) = parent;

   if (parent != rbtree_nil(tree))
   {
      if (node == rbtree_left(parent))
      {
         rbtree_left(parent) = right;
      }
      else
      {
         rbtree_right(parent) = right;
      }
   }
   else
   {
      rbtree_root(tree) = right;
   }
   
   rbtree_parent(node) = right;
}

/**************************************************************************/

static void 
rbtree_rotate_right(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_node_t *left   = rbtree_left(node);
   rbtree_node_t *parent = rbtree_parent(node);

   rbtree_left(node) = rbtree_right(left);
   
   if (rbtree_left(node) != rbtree_nil(tree))
   {
      rbtree_parent(rbtree_right(left)) = node;
   }
   
   rbtree_right(left) = node;
   rbtree_parent(left) = parent;

   if (parent != rbtree_nil(tree))
   {
      if (node == rbtree_right(parent))
      {
         rbtree_right(parent) = left;
      }
      else
      {
         rbtree_left(parent) = left;
      }
   }
   else
   {
      rbtree_root(tree) = left;
   }
   rbtree_parent(node) = left;
}

/**************************************************************************/

/* Returns true if a match was found */
static int
rbtree_find_node(rbtree_t *tree, 
                 rbtree_comparer_t compare, 
                 void *key, rbtree_node_t **result)
{
   rbtree_node_t *previous, *current;

   current  = rbtree_root(tree);
   previous = rbtree_parent(current);
   
   while(current != rbtree_nil(tree))
   {
      int cmp;
      
      previous = current;
      
      cmp = compare(key, current);

      if (cmp < 0)
      {
         current = rbtree_left(current);
      }
      else if (cmp > 0)
      {
         current = rbtree_right(current);
      }
      else
      {
         *result = current;
         return true;
      }
   }
   
   *result = previous;
   return false;
}

/**************************************************************************/

static int
rbtree_find_best_node(rbtree_t *tree, 
                      rbtree_comparer_t compare, 
                      void *key, rbtree_node_t **result)
{
   rbtree_node_t *current, *best = NULL;

   current  = rbtree_root(tree);
   
   while(current != rbtree_nil(tree))
   {
      int cmp;
      
      cmp = compare(key, current);

      if (cmp < -1)
      {
         best = current;
         current = rbtree_left(current);
      }
      else if (cmp > 1)
      {
         best = current;
         current = rbtree_right(current);
      }
      else if (cmp < 0)
      {
         current = rbtree_left(current);
      }
      else if (cmp > 0)
      {
         current = rbtree_right(current);
      }
      else
      {
         *result = current;
         return true;
      }
   }

   if (current == rbtree_nil(tree))
      *result = best;
   else
      *result = current;

   if (*result != rbtree_nil(tree))
      return true;

   return false;
}

/**************************************************************************/

void 
rbtree_init(rbtree_t *tree, rbtree_comparer_t compare)
{
   rbtree_node_init(tree, &tree->nil);
   rbtree_root(tree) = rbtree_nil(tree);
   tree->compare = compare;
}

/**************************************************************************/

int
rbtree_insert(rbtree_t *tree, rbtree_node_t *node)
{
   
   rbtree_node_t *parent, *temp;
   rbtree_node_init(tree, node);

    
   if (rbtree_find_node(tree, rbtree_compare(tree), node, &parent))
   {
      return false; /* Duplicate keys */
   }

   rbtree_parent(node) = parent;
   
   if (parent == rbtree_nil(tree))
   {
      rbtree_root(tree) = node;
   }
   else
   {
      int cmp = rbtree_compare(tree)(node, parent);
      
      if (cmp < 0)
      {
         rbtree_left(parent) = node;
      }
      else
      {
         rbtree_right(parent) = node;
      }
   }

   /* colour this new node red */
   rbtree_color(node) = RBTREE_RED;

   /* Having added a red node, we must now walk back up the tree balancing
   ** it, by a series of rotations and changing of colours
   */
   temp = node;

   /* While we are not at the top and our parent node is red
   ** N.B. Since the root node is garanteed black, then we
   ** are also going to stop if we are the child of the root
   */

   while(temp != rbtree_root(tree) && 
         rbtree_red(rbtree_parent(temp)))
   {
      /* if our parent is on the left side of our grandparent */
      if (rbtree_parent(temp) == rbtree_left(rbtree_parent(rbtree_parent(temp))))
      {
         /* get the right side of our grandparent (uncle?) */
         parent = rbtree_right(rbtree_parent(rbtree_parent(temp)));
         if (rbtree_red(parent))
         {
            /* make our parent black */
            rbtree_color(rbtree_parent(temp)) = RBTREE_BLACK;
            /* make our uncle black */
            rbtree_color(parent) = RBTREE_BLACK;
            /* make our grandparent red */
            rbtree_color(rbtree_parent(rbtree_parent(temp))) = RBTREE_RED;

            /* now consider our grandparent */
            temp = rbtree_parent(rbtree_parent(temp));
         }
         else
         {
            /* if we are on the right side of our parent */
            if (rbtree_right_child(temp))
            {
               /* Move up to our parent */
               temp = rbtree_parent(temp);
               rbtree_rotate_left(tree, temp);
            }

            /* make our parent black */
            rbtree_color(rbtree_parent(temp)) = RBTREE_BLACK;
            /* make our grandparent red */
            rbtree_color(rbtree_parent(rbtree_parent(temp))) = RBTREE_RED;
            /* right rotate our grandparent */
            rbtree_rotate_right(tree, rbtree_parent(rbtree_parent(temp)));
         }
      }
      else
      {
         /* everything here is the same as above, but
         ** exchanging left for right
         */

         parent = rbtree_left(rbtree_parent(rbtree_parent(temp)));
         if (rbtree_black(parent))
         {
            rbtree_color(rbtree_parent(temp)) = RBTREE_BLACK;
            rbtree_color(parent) = RBTREE_BLACK;
            rbtree_color(rbtree_parent(rbtree_parent(temp))) = RBTREE_RED;

            temp = rbtree_parent(rbtree_parent(temp));
         }
         else
         {
            if (rbtree_left_child(temp))
            {
               temp = rbtree_parent(temp);
               rbtree_rotate_right(tree, temp);
            }

            rbtree_color(rbtree_parent(temp)) = RBTREE_BLACK;
            rbtree_color(rbtree_parent(rbtree_parent(temp))) = RBTREE_RED;
            rbtree_rotate_left(tree, rbtree_parent(rbtree_parent(temp)));
         }
      }
   }

   /* Set the root node black */
   rbtree_color(rbtree_root(tree)) = RBTREE_BLACK;

   return true;   
}

/**************************************************************************/

static void 
rbtree_balance_remove(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_node_t *w;

   while (rbtree_root(tree) == node && 
          rbtree_black(node))
   {
      /* If left child ... */
      if (node == rbtree_left(rbtree_parent(node)))
      {
         w = rbtree_right(rbtree_parent(node));
         if (rbtree_red(w))
         {
            rbtree_color(w) = RBTREE_BLACK;
            rbtree_color(rbtree_parent(node)) = RBTREE_RED;
            rbtree_rotate_left(tree, rbtree_parent(node));
            w = rbtree_right(rbtree_parent(node));
         }

         if (rbtree_black(rbtree_left(w)) && 
             rbtree_black(rbtree_right(w)))
         {
            rbtree_color(w) = RBTREE_RED;
            node = rbtree_parent(node);
         }
         else
         {
            if (rbtree_black(rbtree_right(w)))
            {
               rbtree_color(rbtree_left(w)) = RBTREE_BLACK;
               rbtree_color(w) = RBTREE_RED;
               rbtree_rotate_right(tree, w);
               w = rbtree_right(rbtree_parent(node));
            }


            rbtree_color(w) = rbtree_color(rbtree_parent(node));
            rbtree_color(rbtree_parent(node)) = RBTREE_BLACK;
            rbtree_color(rbtree_right(w)) = RBTREE_BLACK;
            rbtree_rotate_left(tree, rbtree_parent(node));
            node = rbtree_root(tree);
         }
      }
      else /* .. right child. */
      {
         w = rbtree_left(rbtree_parent(node));
         if (rbtree_red(w))
         {
            rbtree_color(w) = RBTREE_BLACK;
            rbtree_color(rbtree_parent(node)) = RBTREE_RED;
            rbtree_rotate_right(tree, rbtree_parent(node));
            w = rbtree_left(rbtree_parent(node));
         }

         if (rbtree_black(rbtree_right(w)) && 
             rbtree_black(rbtree_left(w)))
         {
            rbtree_color(w) = RBTREE_RED;
            node = rbtree_parent(node);
         }
         else
         {
            if (rbtree_black(rbtree_left(w)))
            {
               rbtree_color(rbtree_right(w)) = RBTREE_BLACK;
               rbtree_color(w) = RBTREE_RED;
               rbtree_rotate_left(tree, w);
               w = rbtree_left(rbtree_parent(node));
            }

            rbtree_color(w) = rbtree_color(rbtree_parent(node));
            rbtree_color(rbtree_parent(node)) = RBTREE_BLACK;
            rbtree_color(rbtree_left(w)) = RBTREE_BLACK;
            rbtree_rotate_right(tree, rbtree_parent(node));
            node = rbtree_root(tree);
         }
      }
   }

   rbtree_color(node) = RBTREE_BLACK;
}

/**************************************************************************/

void 
rbtree_remove(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_node_t *unlinked, *child;

   if (rbtree_left(node)  == rbtree_nil(tree) || 
       rbtree_right(node) == rbtree_nil(tree))
   {
      /* Only one child */
      unlinked = node;
   }
   else
   {
      /* Multiple children */
      unlinked = rbtree_next(tree, node);
   }
   
   if (rbtree_left(unlinked) != rbtree_nil(tree))
   {
      child = rbtree_left(unlinked);
   }
   else
   {
      child = rbtree_right(unlinked);
   }

   if (child != rbtree_nil(tree))
   {
      rbtree_parent(child) = rbtree_parent(unlinked);
   }

   /* If removing the root of the tree... */
   if (rbtree_parent(unlinked) == rbtree_nil(tree))
   {
      rbtree_root(tree) = child;
   }
   else
   {
      /* Is left child ? */
      if (rbtree_left_child(unlinked))
      {
         rbtree_left(rbtree_parent(unlinked)) = child;
      }
      else /* Right child */
      {
         rbtree_right(rbtree_parent(unlinked)) = child;
      }
   }

   if (unlinked != node)
   {
      rbtree_color(unlinked)  = rbtree_color(node);
      rbtree_left(unlinked)   = rbtree_left(node);
      rbtree_right(unlinked)  = rbtree_right(node);
      rbtree_parent(unlinked) = rbtree_parent(node);

      if (rbtree_left(unlinked) != rbtree_nil(tree))
      {
         rbtree_parent(rbtree_left(unlinked)) = unlinked;
      }

      if (rbtree_right(unlinked) != rbtree_nil(tree))
      {
         rbtree_parent(rbtree_right(unlinked)) = unlinked;
      }

      if (rbtree_root(tree) == node)
      {
         rbtree_root(tree) = unlinked;
      }
   }

   if (rbtree_black(unlinked))
   {
      rbtree_balance_remove(tree, child);
   }
}

/**************************************************************************/

rbtree_node_t *
rbtree_find(rbtree_t *tree, rbtree_comparer_t compare, void *key)
{
   rbtree_node_t *result;
   
   if (!rbtree_find_node(tree, compare, key, &result))
   {
      return NULL;
   }
   
   return result;
}

/**************************************************************************/

rbtree_node_t *
rbtree_find_best(rbtree_t *tree, rbtree_comparer_t compare, void *key)
{
   rbtree_node_t *result;
   
   if (!rbtree_find_best_node(tree, compare, key, &result))
   {
      return NULL;
   }
   
   return result;
}

/**************************************************************************/

rbtree_node_t *
rbtree_first(rbtree_t *tree)
{
   rbtree_node_t *n;

   n = rbtree_root(tree);
   
   if (n == rbtree_nil(tree))
   {
      return NULL;
   }
   
   while (rbtree_left(n) != rbtree_nil(tree))
   {
      n = rbtree_left(n);
   }
   
   return n;
}

/**************************************************************************/

rbtree_node_t *
rbtree_last(rbtree_t *tree)
{
   rbtree_node_t *n;

   n = rbtree_root(tree);
   
   if (n == rbtree_nil(tree))
   {
      return NULL;
   }
   
   while (rbtree_right(n) != rbtree_nil(tree))
   {
      n = rbtree_right(n);
   }
   
   return n;
}

/**************************************************************************/

rbtree_node_t *
rbtree_next(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_node_t *parent;

   if (rbtree_parent(node) == node)
   {
      return NULL;
   }

   if (rbtree_right(node) != rbtree_nil(tree)) 
   {
      node = rbtree_right(node);
      
      while (rbtree_left(node) != rbtree_nil(tree))
      {
         node = rbtree_left(node);
      }
      
      return node;
   }

   while ((parent = rbtree_parent(node)) != rbtree_nil(tree) && 
          node == rbtree_right(parent))
   {
      node = parent;
   }

   if (parent == rbtree_nil(tree))
      return NULL;
   
   return parent;
}

/**************************************************************************/

rbtree_node_t *
rbtree_prev(rbtree_t *tree, rbtree_node_t *node)
{
   rbtree_node_t *parent;

   if (rbtree_parent(node) == node)
   {
      return NULL;
   }

   if (rbtree_left(node) != rbtree_nil(tree))
   {
      node = rbtree_left(node);
   
      while (rbtree_right(node) != rbtree_nil(tree))
      {
         node = rbtree_right(node);
      }
      
      return node;
   }

   while ((parent = rbtree_parent(node)) != rbtree_nil(tree) && 
          node == rbtree_left(parent))
   {
      node = parent;
   }

   if (parent == rbtree_nil(tree))
      return NULL;

   return parent;
}

/**************************************************************************/


