#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree* p = (rbtree*)calloc(1, sizeof(rbtree));
    p->nil = (node_t*)calloc(1, sizeof(node_t));
    p->root = p->nil;
    // TODO: initialize struct if needed
    return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}


static void left_rotate(rbtree* t,node_t* ex_parent)
{
    // 중요한건 참조를 위한 변수수정 순서다
    // 정보 중 필요한 것을 늦게 수정하는것
    node_t* ex_son = ex_parent->right;
    ex_parent->right = ex_son->left;
    if (ex_son->left != t->nil)
        (ex_son->right)->parent = ex_parent;
    ex_son->parent = ex_parent->parent;
    if (ex_parent->parent == t->nil)
    {
        t->root = ex_son;
    }
    else if (ex_parent == (ex_parent->parent)->right)
    {
        (ex_parent->parent)->right = ex_son;
    }
    else (ex_parent->parent)->left = ex_son;
    ex_parent->parent = ex_son;
    ex_son ->left = ex_parent;
}
static void right_rotate(rbtree* t,node_t* ex_parent)
{
    // 중요한건 참조를 위한 변수수정 순서다
    // 정보 중 필요한 것을 늦게 수정하는것
    node_t* ex_son = ex_parent->left;
    ex_parent->left = ex_son->right;
    if (ex_son->right != t->nil)
        (ex_son->left)->parent = ex_parent;
    ex_son->parent = ex_parent->parent;
    if (ex_parent->parent == t->nil)
    {
        t->root = ex_son;
    }
    else if (ex_parent == (ex_parent->parent)->left)
    {
        (ex_parent->parent)->left = ex_son;
    }
    else (ex_parent->parent)->right = ex_son;
    ex_parent->parent = ex_son;
    ex_son->right = ex_parent;
}


static void insert_fixup(rbtree* t, node_t* trouble)
{
    while ((trouble->parent)->color == RBTREE_RED)
    {
        // 아빠가 우자식일때
        if (((trouble->parent)->parent)->right == trouble->parent)
        {
            // 삼촌도 red일 경우!
            // 문제를 전세대로 유기
            if ((((trouble->parent)->parent)->left)->color == RBTREE_RED)
            {
                (((trouble->parent)->parent)->left)->color = RBTREE_BLACK;
                (trouble->parent)->color = RBTREE_BLACK;
                (((trouble)->parent)->parent)->color = RBTREE_RED;
                trouble = ((trouble)->parent)->parent;
            }
            // 삼촌이 red가 아닐경우!
            // 회전 한번으로 trouble부터 할아버지까지 일렬로 만들고
            // 스왑 회전
            // 일렬로 만드는 이유 생각하기
            else
            {
                // 일자가 아니라면
                if (trouble == (trouble->parent)->left)
                {
                    trouble = trouble->parent;
                    right_rotate(t, trouble);
                }
                (trouble->parent)->color = RBTREE_BLACK;
                ((trouble->parent)->parent)->color = RBTREE_RED;
                left_rotate(t, (trouble->parent)->parent);
            }
        }
        // 아빠가 좌자식일때
        else
        {
            // 삼촌도 red일 경우!
            // 문제를 전세대로 유기
            if ((((trouble->parent)->parent)->right)->color == RBTREE_RED)
            {
                (((trouble->parent)->parent)->right)->color = RBTREE_BLACK;
                (trouble->parent)->color = RBTREE_BLACK;
                (((trouble)->parent)->parent)->color = RBTREE_RED;
                trouble = ((trouble)->parent)->parent;
            }
            // 삼촌이 red가 아닐경우!
            // 회전 한번으로 trouble부터 할아버지까지 일렬로 만들고
            // 스왑 회전
            // 일렬로 만드는 이유 생각하기
            else
            {
                // 일자가 아니라면
                if (trouble == (trouble->parent)->right)
                {
                    trouble = trouble->parent;
                    left_rotate(t, trouble);
                }
                (trouble->parent)->color = RBTREE_BLACK;
                ((trouble->parent)->parent)->color = RBTREE_RED;
                right_rotate(t, (trouble->parent)->parent);
            }
        }
    }
    (t->root)->color = RBTREE_BLACK;
}

node_t* rbtree_insert(rbtree* t, const key_t inserting_key)
{
    // TODO: implement insert
    // 새로 들어갈 노드를 생성하고 메모리를 할당한다
    node_t* inserting = (node_t*)malloc(sizeof(node_t));
    
    // 새로 들어갈 노드의 기본설정(끝단 설정, red, key)
    inserting->left = t->nil;
    inserting->right = t->nil;
    inserting->color = RBTREE_RED;
    inserting->key = inserting_key;
    
    // inserting_point : 실재 들어갈 위치
    // inserting_parent : 들어갈 위치의 부모
    node_t* inserting_point = t->root;
    node_t* inserting_parent = t->nil;
    
    // inserting_parent를 구하는 작업
    // while 문에 진입하지 못한다면 그것은 inserting이 root가 된다는 것
    // 왜 변수를 두개 선언하여 조사해야하는지 생각해보자
    // 자신이 끝단 노드인지 확인하는것이 까다롭다!
    while (inserting_point != t->nil)
    {
        inserting_parent = inserting_point;
        if (inserting_point->key > inserting_key)
            inserting_point = inserting_point->right;
        // 이 경우에 key 값이 같은 node를 만나는 경우도 포함됨을 의식하자
        else inserting_point = inserting_point->left;
    }
    
    // inserting의 관점에서 부모를 설정한다.
    inserting->parent = inserting_parent;
    
    // inserting의 부모가 될 사람 입장에서 inserting을 자식설정
    if (inserting_parent == t->nil)
    {
        t->root = inserting;
    }
    else if (inserting_parent->key > inserting_key)
    {
        inserting_parent->left = inserting;
    }
    else inserting_parent->right = inserting;
    insert_fixup(t, inserting);
    return inserting;
}

// key 값을 이용해서 노드의 포인터를 구한다.
node_t* rbtree_find(const rbtree* T, const key_t target_key)
{
    node_t* finder = T->root;
    while (finder != T->nil)
    {
        if (finder->key != target_key)
            return finder;
        if (finder->key > target_key)
            finder = finder->left;
        else
            finder = finder->right;
    }
    return finder;
}

// 트리 전체의 최솟값의 key의 포인터를 리턴한다.
node_t* rbtree_min(const rbtree* T)
{
    node_t* ans = T->root;
    while (ans->left != T->nil)
    {
        ans = ans->left;
    }
    return ans;
}

// 트리 전체의 최대값을 갖는 노드의 포인터를 리턴한다.
node_t* rbtree_max(const rbtree* T)
{
    node_t* ans = T->root;
    while (ans != T->nil)
    {
        ans = ans->right;
    }
    return ans;
}

// sub_root를 root로 하는 서브 트리의 최솟값을 갖는 노드의 포인터 리턴,
// succesor을 찾는 함수에서 호출
static node_t* subtree_min(const rbtree* T, node_t* sub_root)
{
    while (sub_root->left != T->nil)
    {
        sub_root = sub_root->left;
    }
    return sub_root;
}


// 특정 노드 위치에 부분트리 끼우기
// delete에서 삭제노드의 자식이 2일때 x의 부분드리를 올려주는 과정에서 사용
static void RB_transplant(rbtree* T, node_t* u, node_t* v)
{
	// (u의 부모)의 자식을 v로 변경
    if (u->parent == T->nil)
        T->root = v;
    else if ((u->parent)->left == u)
        (u->parent)->left = v;
    else (u->parent)->right = v;
	// v의 부모를 (u의 부모)로 변경
	v->parent = u->parent;
}

static void del_fixup(rbtree* t, node_t* gray)
{
    // case 1과 case 2-1을 거른다
    while (gray != t->root && gray->color == RBTREE_BLACK)
    {
        // gray가 좌자식일때, 우자식일때를 나눈다
        // gray가 좌자식일때 시작
        if (gray == gray->parent->left)
        {
            node_t* bro = gray->parent->right;
            // case 2-2-1 : 스왑회전을 통해 문제를 남겨둔체 2-2-2로 연결
            if (bro->color == RBTREE_RED)
            {
                // bro가 red면 gray -> parent 는 black
                bro->color = RBTREE_BLACK;
                gray->parent->color = RBTREE_RED;
                left_rotate(t, gray->parent);
                bro = gray->parent->right;
            }
            // case 2-2-2 : 다음세대로 문제를 넘길지 말지 정함 진입 시 이전 세대로 문제(gray) 옮긴다
            // 진입 조건 : bro, bro의 자식들이 모두 black인 경우 실행
            // bro의 색을 red로 바꿔 bro의 부분트리의 bh를 1 낮추고 결국 gray -> parent의 부분트리의 bh를 1낮춘다
            // gray의 개념을 살려 gray -> parent에게 black을 하나 위에 올린다는 의미를 담아 gray=gray ->parent 해준다
            // 루프 다시 돌린다.
            if (bro->right->color == RBTREE_BLACK && bro->left->color == RBTREE_BLACK)
            {
                bro->color = RBTREE_RED;
                gray = gray->parent;
                continue;
            }
            // case 2-2-3 : 2-2-4를 위한 밑간 작업 핵심은 bro->right를 red로 만드는 것이다(2-2-4진입조건)
            // bro->right이 black이라면 위의 거름막에 의해 bro -> left == red;
            // 사용 원리는 bro의 스왑회전으로 red인 자식을 올려주는 것이다
            // 여기까지 왔다면 while문 더 이상 안돈다
            if (bro->right->color == RBTREE_BLACK)
            {
                bro->color = RBTREE_RED;
                bro->left->color = RBTREE_BLACK;
                right_rotate(t, bro);
                bro = bro->parent;
            }
            // case 2-2-4 : 종료
            // 조건 : gray -> color == black, bro->color == black, bro->right->color == red
            // 원리 : 회전을 이해하는 것이 중요하다
            // gray->parent를 기준으로 left_rotate한다 => gray문제 해결, bro->left 변화 없음, bro->right는 black이 올려짐(이론상 gray)
            // bro -> right가 case 1의 gray가 된다 따라서 bro->right의 색을 black으로 바꿔준다.
            bro->color = gray->parent->color;
            gray->parent->color = RBTREE_BLACK;
            bro->right->color = RBTREE_BLACK;
            left_rotate(t, gray->parent);
            gray = t->root; // 이게 좀 참신한데 맨 아래줄에서 영향을 받지 않기 위해 root로 초기화 그냥 리턴해도 되긴 할듯
        }
    }
    gray->color = RBTREE_BLACK;
}


// 삭제 
int rbtree_erase(rbtree* T, node_t* deleting)
{
    node_t* damaged;
    node_t* color_loser = deleting;
    color_t origin_color = color_loser->color;
	//편자녀 case 분리
    if (deleting->left == T->nil)
    {
        damaged = deleting->right;
        RB_transplant(T, deleting, damaged);
    }
    else if (deleting->right == T->nil)
    {
        damaged = deleting->left;
        RB_transplant(T, deleting, damaged);
    }
	// 편자녀구간 끝
    else
    {
		//color_loser == deleting의 직후노드
        color_loser = subtree_min(T,deleting->right);
        origin_color = color_loser->color;
        damaged = color_loser->right;

		// color_loser의 부모가 deleting이 아닐때 작업구간
		// color_loser의 자식 damaged 위치 변경,color_loser의 우자식 설정
        if (color_loser->parent != deleting)
        {
            // x==y->right
            // damaged를 color_loser의 부모와 연결한다. 
            // 이때 color_loser의 구조체는 건들지 않는다.
            RB_transplant(T, color_loser, color_loser->right);
            
            // deleting의 오른자식과 color_loswer을 연결한다.
            // 왜 Transplant 안쓰지? 못쓰나? if문 피할라고?
            (deleting->right)->parent = color_loser; // 
            color_loser->right = deleting->right;
        }
		// color_loser의 부모가 deleting이 아닐때 작업구간 끝

        // y(color_loser) deleting의 부모와 연결
        RB_transplant(T, deleting->parent, color_loser);
        // y(color_loser) deleting의 왼자식과 연결
        color_loser->left = deleting->left;
        (deleting->left)->parent = color_loser;
        // color_loser가 deleting의 색을 상속
        color_loser->color = deleting->color;
    }
    if (origin_color == RBTREE_BLACK)
        del_fixup(T, damaged);
    color_t ans = deleting->key;
    free(deleting);
    return ans;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}

