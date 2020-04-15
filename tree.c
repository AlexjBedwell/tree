//
//  tree.c
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "tree.h"
#include "utils.h"

#define MAXLEN 100

//-------------------------------------------------
tnode* tnode_create(const char* word) {
  tnode* p = (tnode*)malloc(sizeof(tnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->count = 1;
  p->left = NULL;
  p->right = NULL;

  return p;
}

void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

tree* tree_create(void) {
  tree* p = (tree*)malloc(sizeof(tree));
  p->root = NULL;
  p->size = 0;

  return p;
}

static void tree_deletenodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_deletenodes(t, p->left);
  tree_deletenodes(t, p->right);
  tnode_delete(p);
  t->size--;
}

void tree_clear(tree* t) {
  tree_delete(t);
  t->root = NULL;
  t->size = 0;
}

void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

bool tree_empty(tree* t) { return t->size == 0; }
size_t tree_size(tree* t) { return t->size; }

static tnode* tree_addnode(tree* t, tnode** p, const char* word) {
  int compare;

  if (*p == NULL) {
    *p = tnode_create(word);
  } else if ((compare = strcmp(word, (*p)->word)) == 0) {
    (*p)->count++;
  } else if (compare < 0) { tree_addnode(t, &(*p)->left, word);
  } else {
    tree_addnode(t, &(*p)->right, word);
  }

  return *p;
}

tnode* tree_add(tree* t, const char* word) {
  tnode* p = tree_addnode(t, &(t->root), word);
  t->size++;

  return p;
}

static void tree_printme(tree* t, tnode* p) {
  printf("%s", p->word);
  if (p->count > 1) { printf(" -- %d", p->count); }
  printf("\n");
}

static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printnodes(t, p->left);
  tree_printme(t, p);
  tree_printnodes(t, p->right);
}

static void tree_printnodes_preorder(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printme(t, p);
  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
}

static void tree_printnodes_postorder(tree*t, tnode* p){
  if(p == NULL){ return; }
  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
  tree_printme(t, p);
}

void tree_print(tree* t) {    // INORDER-printing
  tree_printnodes(t, t->root);
  printf("\n");
}

void tree_print_preorder(tree* t) {
  tree_printnodes_preorder(t, t->root);
  printf("\n");
}

void tree_print_postorder(tree* t){
  tree_printnodes_postorder(t, t->root);
  printf("\n");
}


void tree_test() {
  tree* t = tree_create();
  char c, string[MAXLEN];
  int i = 0;
  while ((c = getchar())!=EOF)
  {
    if(!isalpha(c)) {
      string[i] ='\0';
      if(strcmp(string,"")!=0){
        tree_add(t,string);
        i=0;
        memset(string, 0, MAXLEN);
      }

    }
    else{
      string[i++] = c;
    }
  }
  tree_print_postorder(t);
  printf("is my tree empty? %s\n", yesorno(tree_empty(t)));
  printf("size of tree: %zu\n\n", tree_size(t));

  tree_clear(t);
  printf("is my tree empty now? %s\n", yesorno(tree_empty(t)));
  printf("size of tree: %zu\n\n", tree_size(t));
}


int main(int argc, const char* argv[]) {
  tree_test();
  return 0;
}
