#include "huffcode.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


void deleteTree(){
	//TODO: Write a function that deletes(frees) the data structure
		//used during the decompression algorithm
	freeTree();
}

void deleteList(){
	//TODO: Write a function that deletes(frees) the data
		//structure you were using during your compression algorithm
	freeList();
}

void compress(char * filename, char * output){
	if(!addrs){
		printf("Error: Huffman codes not loaded\n");
		return;
	}
	FILE * f = fopen(filename, "r");
	FILE * o = fopen(output, "wb");
	unsigned char bitstring = 0;
	int bits = 0;
	char * addr;
	char c = (char)fgetc(f);
	while(c != EOF){
		addr = geta(c);
		int i;
		for(i = 0; i < strlen(addr); i++){
			if(addr[i] == '0'){
				bitstring<<=1;
				bits++;
			}else if(addr[i] == '1'){
				bitstring<<=1;
				bitstring = bitstring + 1;
				bits++;
			}else{
				printf("Corrupt binary address\n");
			}

			if(bits == 8){
				fwrite(&bitstring, sizeof(unsigned char), 1, o);
				bits = 0;
				bitstring = 0;
			}
		}
		c = (char)fgetc(f);
	}
	while(bits < 8){
		bitstring<<=1;
		bits++;
	}
	fwrite(&bitstring, sizeof(unsigned char), 1, o);
	bits = 0;
	bitstring = 0;
	fclose(f);
	fclose(o);
}

void decompress(char * filename, char * output){
	if(!h){
		printf("Error: Huffman codes not loaded\n");
		return;
	}
	FILE * f = fopen(filename, "rb");
	FILE * o = fopen(output, "w");
	unsigned char bitstring = 0;
	
	int bits = 8;
	Node * ptr = h;
	while(fread(&bitstring, sizeof(unsigned char), 1,f)){
		while(bits){
			if(ptr->val){
				fprintf(o, "%c", ptr->val);
				ptr = h;
			}else{
				if(bitstring & 0b10000000){
					if(ptr->_1){
						ptr = ptr->_1;
					}
					else{
						ptr = h;
					}
				}else{
					if(ptr->_0){
						ptr = ptr->_0;
					}
					else{
						ptr = h;
					}
				}
				bits--;
				bitstring<<=1;
			}
		}
		if(ptr->val){
				fprintf(o, "%c", ptr->val);
				ptr = h;
		}
		bits = 8;
	}
	fclose(f);
	fclose(o);
}


char *trim(char *str)
{
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  *(end+1) = 0;

  return str;
}

void loadCode(char * filename, int comp){
	if(comp){
		cinit(96);
	}else{
		binit();
	}

	FILE *f = fopen(filename, "r");
    char buff[256];
    while(fgets(buff, 256, f) != NULL){
        char * token = strtok(buff, "=");
        char c = *token;
        token = trim(strtok(NULL, "="));
        if(comp){
        	add(c, token);
        }else{
        	addr(c, token);
        }
    }
    fclose(f);
}
/*	Parse the command inputs:
*
*	argv[0] is always program name
*	-c or -d:			will determine whether to compress or decompress: -c or -d 
*	<PATHTOSOURCE>:		will determine what file to compress(or decompress)(Error if it is a folder)
*	--code:				is a flag to say that the next argument is the file with the huffman codes
*	<PATHTOCODE>:		is the path to the huffman codes file
*	-p:(optional) 		to specify a destination file.
*	<PATHTODEST>:		(if -p is there) specifies the filename to compress(decompress) to.
*		Default destination path(no -p flag) is out.huf and out.dec for compress and decompress, respectively
*		if invalid path, error and exit.
*
*  NOTE:  ARGUMENTS CAN BE IN ANY ORDER, AS LONG AS EACH PATH
			IS PRECEDED BY ITS RESPECTIVE FLAG
*    This means that it could be:
*		./huffman --code <PATHTOCODE> -c <PATHTOSOURCE>    or
*		./huffman -d <PATHTOSOURCE> -p <PATHTODEST> --code <PATHTOCODE>    or
*		./huffman -p <PATHTODEST> -d <PATHTOSOURCE> --code <PATHTOCODE>
* 
*		etc..............
*
*	valid syntax shorthand:
*	./huffman <-c|-d> <PATHTOSOURCE> --code <PATHTOCODE> [-p <PATHTODEST>]
*/
int main(int argc, char const *argv[]) {
	if((argc-1) %2 != 0 || argc == 1){
		printf("Usage: <-c|-d> <PATHTOSOURCE> --code <PATHTOCODE> [-p <PATHTODEST>]\n");
		return -1;
	}
	char * filedecsrc = NULL;
	char * filecompsrc = NULL;
	char * codefile = NULL;
	char * fileout = NULL;

	int i;
	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "--code")){
			codefile = strdup(argv[i+1]);
			break;
		}
	}
	if(!codefile){
		printf("Error: No huffman code file parameter\n");
		return -1;
	}

	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-p")){
			fileout = strdup(argv[i+1]);
			break;
		}
	}

	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-c")){
			filecompsrc = strdup(argv[i+1]);
			if(!fileout){
				fileout = "out.huf";
			}
			break;
		}else if(!strcmp(argv[i], "-d")){
			filedecsrc = strdup(argv[i+1]);
			if(!fileout){
				fileout = "out.dec";
			}
			break;
		}
	}
	if(filedecsrc){
		loadCode(codefile, 0);
		decompress(filedecsrc, fileout);
		deleteTree();
		free(codefile);
		free(filedecsrc);
		free(fileout);
	}else if(filecompsrc){
		loadCode(codefile, 1);
		compress(filecompsrc, fileout);
		deleteList();
		free(codefile);
		free(filecompsrc);
		free(fileout);
	}
	return 0;
}
