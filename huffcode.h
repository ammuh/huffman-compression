//Decompression
typedef struct Node
{
	struct Node *_0;
	struct Node *_1;
	char val;
}Node;

Node * h;

int bload(Node * n);
Node * binit();

char get(char * addr);

int addr(char c, char * addr); //Adds elements recursively

Node * traverse(Node * head, unsigned long bitstring, int * bits, int add);

void printTree(Node * head, char * addr);

//Compression

int ASCII_OFFSET;
int elems;

char ** addrs; //All possible printable ascii characters

int cinit(int num);
int add(char c, char * addr);
char * geta(char c);

void freeList();
void freeTree();