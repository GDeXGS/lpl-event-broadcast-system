#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Link
{
	struct Link *next;
	struct Link *prev;
	char *pname;
};

//新建节点
struct Link *new_node();

//删除节点
bool delete_node(struct Link *node);

//摧毁链表
bool list_destroy(struct Link *head);

//插入
void head_insert(struct Link *head, struct Link *new);

void tail_insert(struct Link *head, struct Link *new);

#endif





