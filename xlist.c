//
//  xlist.c
//  xhttpd
//
//  Created by Peng Yunchou on 13-9-23.
//  Copyright (c) 2013年 Peng Yunchou. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "xlist.h"
struct xlist_node_t {
    struct xlist_node_t* pre;
    void* ud;
    xidentifier_t identifier;
    struct xlist_node_t* next;
};

struct xlist_t {
    struct xlist_node_t* head;
    struct xlist_node_t* tail;
    xlist_keycmp_f key_cmp;
    long size;
};
struct xlist_node_t *xlist_node_new(void *ud,xidentifier_t identifier){
    struct xlist_node_t* n=malloc(sizeof(struct xlist_node_t));
    n->pre=NULL;
    n->ud=ud;
    n->identifier=identifier;
    n->next=NULL;
    return n;
}
int xlist_node_destroy(struct xlist_node_t *n){
    free(n);
    return 0;
}
/***
 *private methods
 ***/
struct xlist_node_t *_find_node_by_index(struct xlist_t* l,long index){
    int i=0;
    struct xlist_node_t *n=l->head;
    for (i=0; i<index; i++) {
        n=n->next;
    }
    return n;
}
struct xlist_node_t *_find_node_by_identifier(struct xlist_t* l,xidentifier_t identifier){
    struct xlist_node_t *n=NULL;
    struct xlist_node_t *p=l->head;
    for (; p!=NULL;p=p->next) {
        if (l->key_cmp(p->identifier,identifier)==0) {
            n=p;
            break;
        }
    }
    return n;
}
int _xlist_remove_node(struct xlist_t* l,struct xlist_node_t* findnode){
    if (findnode==NULL) {
        return 0;
    }
    struct xlist_node_t *prenode=findnode->pre;
    struct xlist_node_t *nextnode=findnode->next;
    if (prenode==NULL&&nextnode==NULL) {
        l->head=NULL;
        l->tail=NULL;
    }else if(prenode==NULL&&nextnode!=NULL){//head
        l->head=nextnode;
        l->head->pre=NULL;
    }else if(prenode!=NULL&&nextnode==NULL){//tail
        l->tail=prenode;
        l->tail->next=NULL;
    }else{//middle
        prenode->next=nextnode;
        if (nextnode!=NULL) {
            nextnode->pre=prenode;
        }
    }
    xlist_node_destroy(findnode);
    l->size--;
    return 0;
}
/***
 *end
 *private methods
 ***/
struct xlist_t* xlist_new(xlist_keycmp_f cmp){
    assert(cmp!=NULL);
    struct xlist_t* l=malloc(sizeof(struct xlist_t));
    l->head=NULL;
    l->tail=NULL;
    l->key_cmp=cmp;
    return l;
}
void xlist_destroy(struct xlist_t* list){
    free(list);
}
int xlist_push(struct xlist_t* l,xidentifier_t identifier,void *ud){
    struct xlist_node_t* n=xlist_node_new(ud, identifier);
    if (l->size==0) {
        n->pre=NULL;
        n->next=NULL;
        l->head=n;
        l->tail=n;
    }else{
        struct xlist_node_t* tail=l->tail;
        tail->next=n;
        n->next=NULL;
        n->pre=tail;
    }
    l->size++;
    return 0;
}
void* xlist_pop(struct xlist_t* l){
    void* ud=NULL;
    if (l->size>0) {
        struct xlist_node_t* head=l->head;
        l->head=head->next;
        l->head->pre=NULL;
        ud=head->ud;
        xlist_node_destroy(head);
        l->size--;
    }
    return ud;
}

int xlist_insert(struct xlist_t* l,xidentifier_t identifier,void *ud,long index){
    if (index<0) {
        index=0;
    }
    if (index>l->size) {
        index=l->size;
    }
    struct xlist_node_t* n=xlist_node_new(ud, identifier);
    if (l->size==0) {
        n->pre=NULL;
        n->next=NULL;
        l->head=n;
        l->tail=n;
    }else{
        if (index==0) {//head
            n->next=l->head;
            n->pre=NULL;
            l->head=n;
        }else if(index>=l->size){//last
            l->tail->next=n;
            n->pre=l->tail;
            n->next=NULL;
            l->tail=n;
        }else{//middle
            //find the node
            struct xlist_node_t *before_node=_find_node_by_index(l,index);
            //inser node after the before_node
            struct xlist_node_t *after_node=before_node->next;
            before_node->next=n;
            n->pre=before_node;
            n->next=after_node;
            if (after_node!=NULL) {
                after_node->pre=n;
            }
        }
    }
    l->size++;
    return 0;
}
void* xlist_get_by_index(struct xlist_t* l,long index){
    struct xlist_node_t* n= _find_node_by_index(l,index);
    return n->ud;
}
void* xlist_get_by_identifier(struct xlist_t* l,xidentifier_t identifier){
    struct xlist_node_t* n= _find_node_by_identifier(l, identifier);
    return n->ud;
}

int xlist_remove_by_index(struct xlist_t* l,long index){
#if DEBUG
    printf("remvoe:%ld\n",index);
#endif
    struct xlist_node_t *findnode=_find_node_by_index(l, index);
    return _xlist_remove_node(l, findnode);
}
int xlist_remove_by_identifier(struct xlist_t* l,xidentifier_t identifier){
    struct xlist_node_t* findnode=_find_node_by_identifier(l, identifier);
    return _xlist_remove_node(l, findnode);
}
long xlist_count(struct xlist_t* l){
    return l->size;
}
int xlist_number_key_cmp(xidentifier_t identifier1,xidentifier_t identifier2){
    return (identifier1==identifier2)?0:1;
}