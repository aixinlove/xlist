//
//  xlist.h
//  xhttpd
//
//  Created by Peng Yunchou on 13-9-23.
//  Copyright (c) 2013年 Peng Yunchou. All rights reserved.
//

#ifndef xhttpd_xlist_c_h
#define xhttpd_xlist_c_h

#define xidentifier_t int
typedef int (*xlist_keycmp_f)(xidentifier_t,xidentifier_t);
struct xlist_t;
struct xlist_t* xlist_new(xlist_keycmp_f);
void xlist_destroy(struct xlist_t* list);
int xlist_push(struct xlist_t* l,xidentifier_t identifier,void *ud);
//pop first userdata only
void* xlist_pop(struct xlist_t* l);
int xlist_insert(struct xlist_t* l,xidentifier_t identifier,void *ud,long index);
void* xlist_get_by_index(struct xlist_t* l,long index);
void* xlist_get_by_identifier(struct xlist_t* l,xidentifier_t identifier);
int xlist_remove_by_index(struct xlist_t* l,long index);
int xlist_remove_by_identifier(struct xlist_t* l,xidentifier_t identifier);
long xlist_count(struct xlist_t* l);
int xlist_number_key_cmp(xidentifier_t identifier1,xidentifier_t identifier2);

#endif
