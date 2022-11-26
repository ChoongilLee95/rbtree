#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);


// 노드 검색
// 트리 전체의 최소 노드
node_t* rbtree_min(const rbtree*);
// 트리 전체의 최대 노드
node_t* rbtree_max(const rbtree*);
// key값에 맞는 노드
node_t* rbtree_find(const rbtree*, const key_t);
// 노드 삭제
int rbtree_erase(rbtree*, node_t*);
// 노드 추가
node_t* rbtree_insert(rbtree*, const key_t);
// array화
int rbtree_to_array(const rbtree*, key_t*, const size_t);

#endif  // _RBTREE_H_
