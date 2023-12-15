#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include<stdlib.h>

//structure used to define a node
typedef struct node_t {
	struct node_t* left, * right;	//왼쪽 오른쪽 자식
	int freq;						//빈도
	char c;							//문자
} *node;

//global variables
int n_nodes = 0, qend = 1; 		//global variables for keep track of no.of nodes and end of the que
struct node_t pool[256] = { {0} };        //pool of nodes, 문자 수
node qqq[255], * q = qqq - 1;      	//the priority que ????
char* code[128] = { 0 };
char* buffer = NULL;//a string array of the codes for each letter, 각 문자를 담은 문자열, 최대 1024자의 문자 처리...?
int input_data = 0, output_data = 0;		//인풋 아웃풋???


//function used to create a new node
node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq != 0) {
		n->c = c;			//assign the character 'c' to the character of the node (eventually a leaf)
		n->freq = freq;			//assign frequency
	}
	else {
		n->left = a, n->right = b;	//if there is no frequency provided with the invoking
		n->freq = a->freq + b->freq;	//the removed nodes at the end of the que will be added to left and right
	}
	return n;
}

//function ued to insert a node into the 우선순위 큐
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}

node qremove()
{
	int i, l;
	node n = q[i = 1];

	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;	//return the node
}

//go along the builded huffman tree and assign the code for each character
void build_code(node n, char* s, int len)
{
	static char* out = buffer;
	if (n->c) {
		s[len] = 0;		//if the provided node is a leaf (end node)
		strcpy(out, s);		//it contains a character
		code[(int)n->c] = out;	//therefore the code is copied in to the relevant character.
		out += len + 1;		//out pointer is incremented
		return;
	}

	s[len] = '0'; build_code(n->left, s, len + 1);	//recurring is used to write out the code
	s[len] = '1'; build_code(n->right, s, len + 1);	//if right add a 1 and if right add a 0
}

void import_file(FILE* fp_in, unsigned int* freq) {//encode 초반에 실행됨, 원본 텍스트를 읽어옴, freq 완성됨
	char c, s[16] = { 0 };		//temporary variables
	int i = 0;
	printf("File Read:\n");
	while ((c = fgetc(fp_in)) != EOF) {
		freq[(int)c]++;		//read the file character by character and increment the particular frequency
		putchar(c);
	}
	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));//insert new nodes into the que if there is a frequency
	while (qend > 2)
		qinsert(new_node(0, 0, qremove(), qremove()));	//build the tree
	build_code(q[1], s, 0);		//build the code for the characters
}

void encode(FILE* fp_in, FILE* fp_zz, unsigned int* freq) {

	char in, c, temp[20] = { 0 };//왜 20자만??
	int i, j = 0, k = 0, lim = 0;
	rewind(fp_in);//이거는 앞에서 읽었기에 다시 앞으로 보내는 거
	for (i = 0; i < 128; i++) {
		if (freq[i])	lim += (freq[i] * strlen(code[i]));
	}
	output_data = lim;			//The output data is equal to the limit
	fprintf(fp_zz, "%04d\n", lim);
	printf("\nEncoded:\n");
	for (i = 0; i < lim; i++) {
		if (temp[j] == '\0') {
			in = fgetc(fp_in);
			strcpy(temp, code[in]);
			printf("%s", code[in]);
			j = 0;
		}
		if (temp[j] == '1')
			c = c | (1 << (7 - k));       //shifts 1 to relevant position and OR with the temporary char
		else if (temp[j] == '0')
			c = c | (0 << (7 - k));       //shifts 0 to relevant position and OR with the temporary char
		else
			printf("ERROR: Wrong input!\n");
		k++;                            // k is used to divide the string into 8 bit chunks and save
		j++;
		if (((i + 1) % 8 == 0) || (i == lim - 1)) {
			k = 0;                  //reset k
			fputc(c, fp_zz);        //save the character
			c = 0;                  //reset character
		}
	}
	putchar('\n');
}

void decode(FILE* fp_zz, FILE* fp_out) {//추가
	int i = 0, lim = 0, j = 0;
	char c;
	node n = q[1];

	fscanf(fp_zz, "%d", &lim);			//get the length of the bit stream from header
	fseek(fp_zz, 1, SEEK_CUR);			//seek one position to avoid new line character of the header

	printf("Decoded : \n");
	for (i = 0; i < lim; i++) {
		if (j == 0)
			c = fgetc(fp_zz);
		//if the anding of the character with b1000 0000 is true then,
		if (c & 128)	n = n->right;		//1 go right
		else 		n = n->left;            //else go left
		if (n->c) {                               //until a leaf (node with a character) meets
			putchar(n->c);                  //spit that character out and
			fputc(n->c, fp_zz);             //save the character in file
			n = q[1];                       //reset the que
		}
		c = c << 1;				//shift the character by 1
		if (++j > 7)
			j = 0;
	}

	putchar('\n');
	if (q[1] != n) printf("garbage input\n");	//the last node should end with a character which reset the que
}

void print_code(unsigned int* freq) {
	int i;
	printf("\n---------CODE TABLE---------\n----------------------------\nCHAR  FREQ  CODE\n----------------------------\n");
	for (i = 0; i < 128; i++) {
		if (isprint((char)i) && code[i] != NULL && i != ' ')
			printf("%-4c  %-4d  %16s\n", i, freq[i], code[i]);
		else if (code[i] != NULL) {
			switch (i) {
			case '\n':
				printf("\\n  ");
				break;
			case ' ':
				printf("\' \' ");
				break;
			case '\t':
				printf("\\t  ");
				break;
			default:
				printf("%0X  ", (char)i);
				break;
			}
			printf("  %-4d  %16s\n", freq[i], code[i]);
		}
	}
	printf("----------------------------\n");

}

void import_table(FILE* fp_zz, unsigned int* freq) { //여기부터 다름
	char c;						//temporary variable
	int i = 0;
	int count = 0;
	while (count < 128 && (c = fgetc(fp_zz)) != EOF) {
		// 읽은 문자에 대한 처리 (예: 화면에 출력)
		freq[i++] = (unsigned char)c; //get the values from the .zz to update the huffman tree
		count++;
	}
	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));	//insert new nodes into the que if there is a frequency
	while (qend > 2)
		qinsert(new_node(0, 0, qremove(), qremove()));		//build the tree
}

int main(int argc, char* argv[]) {

	char file_name[50] = { 0 };				//file name, 파일 이름은 문제 없음
	unsigned int freq[128] = { 0 }, i;		//frequency of the letters, 몇억대라 빈도수는 문제 없음
	char mode[2] = { 0 };					//암호화 복호화 모드 추가
	char ENCODE[] = "-c";
	char DECODE[] = "-d";

	if (argc == 3) {
		strcpy(mode, argv[1]); // -c 또는 -d
		strcpy(file_name, argv[2]);		//commandline argument directly allows to compress the file

	}
	else if (argc > 3) {
		printf("Too many arguments supplied.\n");
	}
	else {
		//printf("Please enter the file to be compressed\t: "); //else a prompt comes to enter the file name
		scanf("%s", file_name);
	}

	if (strcmp(mode, ENCODE)) {
		//여기부터는 encode
		//import the file into the program and update the huffman tree
		FILE* fp_in, * fp_zz;				//FIFO pointers
		if ((fp_in = fopen(file_name, "r")) == NULL) {	//open the file stream
			printf("\nERROR: No such file\n");
			return 0;
		}
		long file_size;
		fseek(fp_in, 0, SEEK_END);
		file_size = ftell(fp_in);
		rewind(fp_in);
		buffer = (char*)malloc(file_size * sizeof(char));

		import_file(fp_in, freq);			//import the file and fills frequency array
		print_code(freq);				//print the code table

		//Encode and save the encoded file
		strcat(file_name, ".zz");			//encoded file is saved in .huffman extension
		fp_zz = fopen(file_name, "w");
		for (int i = 0; i < 128; i++) {
			fprintf(fp_zz, "%c", (char)freq[i]);	//write the frequency to .table file
		}
		encode(fp_in, fp_zz, freq);

		fclose(fp_in);
		fclose(fp_zz);
		free(buffer);
		for (i = 0; i < 128; i++)	input_data += freq[i];	//calculate input bytes

		printf("\nInput bytes:\t\t%d\n", input_data);
		output_data = (output_data % 8) ? (output_data / 8) + 1 : (output_data / 8);
		printf("Output bytes:\t\t%d\n", output_data);

		printf("\nCompression ratio:\t%.2f%%\n\n\n", ((double)(input_data - output_data) / input_data) * 100);
		return 0;
	}
	else if (strcmp(mode, DECODE)) {//여기서부터는 decode
		//import the frequency table and compressed from file and create the huffman tree
		FILE* fp_zz, * fp_yy;				//FIFO pointers
		if ((fp_zz = fopen(file_name, "r")) == NULL) {
			printf("\nERROR: No such file\n");
			return 0;
		}

		import_table(fp_zz, freq);			//import the file and fills frequency array
		strcat(file_name, ".yy");			//encoded file is saved in .huffman extension
		fp_yy = fopen(file_name, "w");
		decode(fp_zz, fp_yy);			//decode the file and save

		fclose(fp_zz);				//close the file pointer for .huffman file
		fclose(fp_yy);					//close file pointer for output file
		return 0;
	}
}