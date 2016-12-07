#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "huffcode.h"

//Decompression
Node * binit(){
	h = malloc(sizeof(Node));
	h->_0 = NULL;
	h->_1 = NULL;
	return h;
}

int bload(Node * n){
	h = n;
	return 0;
}

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

char get(char * addr){
	int len = strlen(addr);
	Node * n = traverse(h, genBitString(addr), &len, 0);
	if(!len){
		return n->val;
	}else{
		return -1;
	}
}

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
} //Adds elements recursively

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

void strrev(char *p)
{
  char *q = p;
  while(q && *q) ++q;
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}

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

//Compression

int cinit(int n){
	addrs = malloc(n*sizeof(char *));
	ASCII_OFFSET = 32;
	elems = n;
	return 0;
}

int add(char c, char * addr){
	if(c <32 || c > 127){
		printf("Error: ASCII Value out of printable range\n");
		return -1;
	}
	addrs[((int)c) - ASCII_OFFSET] = strdup(addr);
	return 0;
}

char * geta(char c){
	return addrs[((int)c) - ASCII_OFFSET];
}

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

void freeBranch(Node * n){
	if(n){
		freeBranch(n->_0);
		freeBranch(n->_1);
		free(n);
	}
}

void freeTree(){
	if(!h){
		return;
	}
	freeBranch(h->_0);
	freeBranch(h->_1);
	free(h);
}

