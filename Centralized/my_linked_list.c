#include <stdio.h>
#include <stdlib.h>
#include "_LINKED_LIST.h"

List* create_list(){
    /*******************************************************************
    * DESCRIPTION : Retuns an initialized LinkedList.
    * 
    */

	List* temp = (List*) malloc(sizeof(List));
	if(temp == NULL){
		return NULL;
	}else{
		temp -> head = NULL;
		temp -> tail = NULL;
		temp -> size = 0;
		return temp;
	}
}

Node* create_node(Element* data){
    /*******************************************************************
    * DESCRIPTION : Retuns an initialized LinkedList node.
    * 
    */

	Node* temp = (Node*) malloc(sizeof(Node));
	if(temp == NULL){
		return NULL;
	}else{
		temp -> ele = data;
		temp -> next = NULL;
		return temp;
	}
}

List* insert_in_order(List* list, Element* data){
    /*******************************************************************
    * DESCRIPTION : Insert a node into the LinkedList.
    * INPUT :     
    *           [1] list : Pointer to the head of LinkedList.
    *           [2] data : data that needs to be inserted.
    * OUTPUT :
    *           [1] Returns pointer to the head of the LinkedList.
    * 
    */
	Node* current;
	Node* new_node = create_node(data);
    if (list -> size == 0){
        list -> head = new_node;
        list -> tail = new_node;
        list -> size ++;
    }else if(list -> head -> ele -> frequency <= new_node -> ele -> frequency){
    	new_node -> next = list -> head;
    	list -> head = new_node;
    	list -> size ++;
    }else{
        current = list -> head;
        while (current -> next != NULL && current -> next -> ele -> frequency > new_node -> ele -> frequency){
            current = current -> next;
        }
        new_node -> next = current -> next;
        current -> next = new_node;
        if(current -> next == NULL){
        	list -> tail = new_node;
        }
        list -> size ++;
    }
}

List* add_to_end(List* list, Element* data){
    /*******************************************************************
    * DESCRIPTION : Insert a node at the end the LinkedList.
    * INPUT :     
    *           [1] list : Pointer to the head of LinkedList.
    *           [2] data : data that needs to be inserted.
    * OUTPUT :
    *           [1] Returns pointer to the head of the LinkedList.
    * 
    */
    Node* new_node = create_node(data);
    if(list -> size == 0){
        list -> head = new_node;
        list -> tail = new_node;
        list -> size ++;
    }else{
        list -> tail -> next = new_node;
        list -> tail = new_node;
        list -> size ++;
    }
    return list;
}

List* delete_from_end(List* list){
    /*******************************************************************
    * DESCRIPTION : Deletes a node from the end of the LinkedList.
    * INPUT :     
    *           [1] list : Pointer to the head of LinkedList.
    * OUTPUT :
    *           [1] Returns pointer to the head of the LinkedList.
    * 
    */
	Node* del_node = list -> tail;
	Node* temp = list -> head;

	if(list -> size == 0){
		return NULL;
	}else if(list -> size == 1){
		list -> head = NULL;
		list -> tail = NULL;
		free(del_node);
	}else{
		while(temp -> next -> next != NULL){
			temp = temp -> next;
		}

		temp -> next = NULL;
		list -> tail = temp;
		free(del_node);
	}
	list -> size --;

	return list;
}

List* delete_from_front(List* list){
    /*******************************************************************
    * DESCRIPTION : Deletes a node from the front of the LinkedList.
    * INPUT :     
    *           [1] list : Pointer to the head of LinkedList.
    * OUTPUT :
    *           [1] Returns pointer to the head of the LinkedList.
    * 
    */
	Node* del_node = list -> head;

	if(list -> size == 0){
		return NULL;
	}else if(list -> size == 1){
		list -> head = NULL;
		list -> tail = NULL;
		free(del_node);
	}else{
		list -> head = list -> head -> next;
		free(del_node);
	}
	list -> size --;

	return list;
}

bool is_empty(List* list){
    /*******************************************************************
    * DESCRIPTION : Check the whether the list is empty
    * INPUT :     
    *           [1] list : Pointer to the head of LinkedList.
    * OUTPUT :
    *           [1] Returns a boolean
    * 
    */
	if(list -> size == 0){
		return true;
	}else{
		return false;
	}
}

void print_list(List* list){
    /*******************************************************************
    * DESCRIPTION : Prints the LinkedList
    * INPUT :     
    *           [1] list : Pointer to the head of LinkedList.
    * 
    */
	Node* temp = list -> head;
	while(temp != NULL){
		printf("%d::%s\n", temp -> ele -> frequency, temp -> ele -> doc_name);
		temp = temp -> next;
	}
	printf("\n");
}

List* mergeLists(List* list1, List* list2){
    /*******************************************************************
    * DESCRIPTION : Merge two LinkedList such that they remain in sorted order
    * INPUT :     
    *           [1] list1 : Pointer to the head of first LinkedList.
    *           [1] list2 : Pointer to the head of second LinkedList.
    * OUTPUT :
    *           [1] Returns pointer to the head of the merged LinkedList.
    * 
    */
    List* ret = create_list();
    Node* iter1 = list1 -> head;
    Node* iter2 = list2 -> head;

    while(iter1 != NULL && iter2 != NULL){
        if(iter1 -> ele -> frequency > iter2 -> ele -> frequency){
            if(!ret -> head){
                ret -> head = iter1;
                ret -> tail = iter1;
                iter1 = iter1 -> next;
                ret -> tail -> next = NULL;
            }else{
                ret -> tail -> next = iter1;
                ret -> tail = iter1;
                iter1 = iter1 -> next;
                ret -> tail -> next = NULL;
            }
        }else{
            if(!ret -> head){
                ret -> head = iter2;
                ret -> tail = iter2;
                iter2 = iter2 -> next;
                ret -> tail -> next = NULL;
            }else{
                ret -> tail -> next = iter2;
                ret -> tail = iter2;
                iter2 = iter2 -> next;
                ret -> tail -> next = NULL;
            }
        }
    }

    if(iter1 == NULL){
        if(list1 -> size == 0){
            ret -> head = iter2;
            ret -> tail = list1 -> tail;
        }else{
            ret -> tail -> next = iter2;
            while(ret -> tail -> next != NULL){
                ret -> tail = ret -> tail -> next;
            }
        }
    }

    if(iter2 == NULL){
        if(list2 -> size == 0){
            ret -> head = iter1;
            ret -> tail = list2 -> tail;
        }else{
            ret -> tail -> next = iter1;
            while(ret -> tail -> next != NULL){
                ret -> tail = ret -> tail -> next;  
            }
        }
    }

    return ret;
}


/*
int main(){

	Element* data = (Element*) malloc(sizeof(Element));
	Element* data1 = (Element*) malloc(sizeof(Element));
	Element* data2 = (Element*) malloc(sizeof(Element));

    Element* data3 = (Element*) malloc(sizeof(Element));
	Element* data4 = (Element*) malloc(sizeof(Element));
	Element* data5 = (Element*) malloc(sizeof(Element));

	data -> frequency = 9;
	data1 -> frequency = 5;
	data2 -> frequency = 2;
    data3 -> frequency = 1;
	data4 -> frequency = 7;
	data5 -> frequency = 3;   

	List* list = create_list();
	list = insert_in_order(list, data);
	list = insert_in_order(list, data1);
	list = insert_in_order(list, data2);
    list = insert_in_order(list, data3);
    list = insert_in_order(list, data4);
	print_list(list);

    List* list2 = create_list();
	// list2 = insert_in_order(list2, data3);
	// list2 = insert_in_order(list2, data4);
	// list2 = insert_in_order(list2, data5);
	print_list(list2);

    List* l3 = mergeLists(list, list2); 
    print_list(l3);
	return 0;
}
*/