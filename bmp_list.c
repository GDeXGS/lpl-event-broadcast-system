#include <bmp_list.h>

//新建节点
struct Link *new_node()
{
	//申请结构体空间
	struct Link *node = (struct Link *)malloc(sizeof(struct Link));
	if(node == NULL)
	{
		perror("");
	}
	
	//初始化
	node->prev = node;
	node->next = node;
	node->pname = NULL;
	
	return node;
}

//删除节点
bool delete_node(struct Link *node)
{
	if(node == NULL) return true;
	
	node->prev->next = node->next;
	node->next->prev = node->prev;
	
	free(node->pname);
	free(node);
	return true;
}

//摧毁链表
bool list_destroy(struct Link *head)
{
	if(head == NULL) return true;
	struct Link *pos = head;
	while(pos->next != head)
	{
		delete_node(pos->next);
	}
	delete_node(head);
	return true;
}

//插入
void head_insert(struct Link *head, struct Link *new)
{
	new->prev = head->prev;
	new->next = head;
	head->prev->next = new;
	head->prev = new;
}

void tail_insert(struct Link *head, struct Link *new)
{
	new->prev = head;
	new->next = head->next;
	head->next->prev = new;
	head->next = new;
}


