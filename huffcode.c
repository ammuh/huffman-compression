#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "huffcode.h"

//Decompression

/*
	Initializes a BTree
*/
Node * binit(){
	h = malloc(sizeof(Node));
	h->_0 = NULL;
	h->_1 = NULL;
	return h;
}
/*
	Loads an existing BTree
*/
int bload(Node * n){
	h = n;
	return 0;
}
/*
	Creates bitstring from a given BTree address in string form
*/
unsigned long genBitString(char * addr){
	int i = 0;
	unsigned long bitstring = 0;
	while(addr[i] != '\0'){
		if(addr[i] == '1'){
			bitstring += (1<<i);
		}else if(addr[i] == '0'){
		}else{
			printf("Invalid B-Tree address\n");;
			return 0;
		}
		i++;
	}
	return bitstring;

}
/*
	Returns character at a given address
*/
char get(char * addr){
	int len = strlen(addr);
	Node * n = traverse(h, genBitString(addr), &len, 0);
	if(!len){
		return n->val;
	}else{
		return -1;
	}
}
/*
	Adds a char value at the root of a tree, if childs don't exist, they are created
	at runtime
*/
int addr(char c, char * addr){
	int len = strlen(addr);
	Node * n = traverse(h, genBitString(addr), &len, 1);
	if(!n->_0 && !n->_1){
		n->val = c;
		return 0;
	}else{
		printf("Address is not a root of the tree.\n");
		return -1;
	}
}

/*
	Traverses a tree based on a given bitstring, returns farthest element it got to.
	Has option for recursively adding elements or stopping at lowest BTree level.
*/

Node * traverse(Node * head, unsigned long bitstring, int * bits, int add){
	Node * elem = head;
	while((*bits)){
		(*bits)--;
		if(bitstring & 0b1){
			if(!elem->_1){
				if(add){
					elem->_1 = malloc(sizeof(Node));
					elem->_1->_0 = NULL;
					elem->_1->_1 = NULL;
				}else{
					return elem;
				}
			}
			elem = elem->_1;
		}else{
			if(!elem->_0){
				if(add){
					elem->_0 = malloc(sizeof(Node));
					elem->_0->_0 = NULL;
					elem->_0->_1 = NULL;
				}else{
					return elem;
				}
			}
			elem = elem->_0;
		}
		bitstring >>= 1;
	}
	return elem;
}

//Helper method for printing a given tree
void strrev(char *p)
{
  char *q = p;
  while(q && *q) ++q;
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}

//Helper method that prints a given BTree recursively
void printTree(Node * elem, char * addr){
	if(!elem){
		elem = h;
	}
	if(!elem->_0 && !elem->_1){
		char buf[15];
		strcpy(buf, addr);
		strrev(buf);
		printf("Value: '%c' at address: '%s'\n", elem->val, buf);
	}else{
		if(elem->_0){
			char add[10];
			strcpy(add, "0");
			strcat(add, addr);
			printTree(elem->_0, add);
		}
		if(elem->_1){
			char add[10];
			strcpy(add, "1");
			strcat(add, addr);
			printTree(elem->_1, add);
		}
	}
}

//Recursive method that frees a given element and all its children
void freeBranch(Node * n){
	if(n){
		freeBranch(n->_0);
		freeBranch(n->_1);
		free(n);
	}
}

/*
	Frees memory from BTree
*/
void freeTree(){
	if(!h){
		return;
	}
	freeBranch(h->_0);
	freeBranch(h->_1);
	free(h);
}

//Compression

/*
	Intializes array of bitcodes
*/
int cinit(int n){
	addrs = malloc(n*sizeof(char *));
	ASCII_OFFSET = 32;
	elems = n;
	return 0;
}

/*
	Adds an address/bitcode for a given character
*/
int add(char c, char * addr){
	if(c <32 || c > 127){
		printf("Error: ASCII Value out of printable range\n");
		return -1;
	}
	addrs[((int)c) - ASCII_OFFSET] = strdup(addr);
	return 0;
}

/*
	Gets the address/bitcode for a certain character. Since the array indexes are based on the
	ASCII codes, the bitcode is added in 1 operation.
*/
char * geta(char c){
	return addrs[((int)c) - ASCII_OFFSET];
}

/*
	Frees the array of bitcodes
*/
void freeList(){
	if(!elems){
		return;
	}
	int i;
	for(i = 0; i < elems; i++){
		if(addrs[i]){
			free(addrs[i]);
		}
	}
	free(addrs);
}