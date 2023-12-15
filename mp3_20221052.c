#include <stdio.h>
#include <stdlib.h>
#define MAX 99999
typedef struct TreeNode {
    char ch; // ���� 
    int weight; // ������ �󵵼�(����ġ)
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;
typedef TreeNode* TreeNode_ptr;
typedef struct {
    TreeNode_ptr htree; // ���� Ʈ�� 
    char ch; // ���� 
    int key; // ������ �󵵼�(����ġ) 
}element;
typedef struct {
    element heap[MAX];
    int heap_size;
}HeapType;
typedef HeapType* HeapType_ptr;
HeapType_ptr Heap_init() {
    HeapType_ptr h = (HeapType_ptr)malloc(sizeof(HeapType));
    h->heap_size = 0;
    return h;
}
// ���� ����� ������ heap_size�� ���� h�� item�� �����Ѵ�.
// ���� �Լ�
void insert_min_heap(HeapType_ptr h, element item) {
    int i;
    i = ++(h->heap_size);
    // Ʈ���� �Ž��� �ö󰡸鼭 �θ� ���� ���ϴ� ����
    // �迭�� Ʈ���� �����ϴϱ� �θ� ����� idx�� 1 
    while ((i != 1) && (item.key < h->heap[i / 2].key)) {
        h->heap[i] = h->heap[i / 2]; // �θ� �� ĭ ���� 
        i /= 2;
    }
    h->heap[i] = item; // ���Ե� ��ġ�� Ȯ������ ������ ���ο� ��带 ���� 
}
// ���� �Լ� 
element delete_min_heap(HeapType_ptr h) {
    int parent, child;
    element item, temp;
    item = h->heap[1]; // ��Ʈ ��� ���� ��ȯ�ϱ� ���� item������ �ű��.
    temp = h->heap[(h->heap_size)--]; // ���� ��� ���� temp������ �ű�� �� ����� 1 ���δ�. (ó������ ��Ʈ�� ��ġ���ִٰ� ����) 
    parent = 1;
    child = 2; // ��Ʈ�� ���� �ڽĺ��� ���Ѵ�. 
    // �ڽ��� ���� ����� �ʾ����� �ݺ��� ���� 
    while (child <= h->heap_size) {
        // �� �ڽĳ�尡 ������ �� ���� �ڽĳ�带 ã�´�.
        if ((h->heap[parent * 2].key >= 0) && (h->heap[(parent * 2) + 1].key >= 0)) {
            if ((h->heap[parent * 2].key) < (h->heap[(parent * 2) + 1].key))
                child = parent * 2;
            else
                child = (parent * 2) + 1;
        }
        // ���� ��带 ���� �ڽ��� �θ� ����� ��ġ�� �ִٰ� �������� ��, ���� �۰ų� ������ �� ������ �ʿ䰡 X
        if (temp.key <= h->heap[child].key)
            break;
        // �θ� ��� �ڸ��� �ڽ� ����� Ű ���� ���� 
        h->heap[parent] = h->heap[child];
        // �� �ܰ� �Ʒ��� �̵�
        parent = child;
        child *= 2;
    }
    // ���� ��带 ���� �ڽ��� �θ� ����� ��ġ�� �ִٰ� �������� ��, �ڽ� ���̶� �۰ų� ���� -> ������ �θ� ��忡 ���� 
    // (Or) child>h->heap_size��, ��ġ�ؾ� �� ��忡 ���� 
    h->heap[parent] = temp;
    return item;
}
// ���� Ʈ�� ���� �Լ� 
TreeNode_ptr make_Htree(TreeNode_ptr left, TreeNode_ptr right) {
    TreeNode_ptr node = (TreeNode_ptr)malloc(sizeof(TreeNode));
    node->left = left;
    node->right = right;
    return node;
}
// �� ��� �Ǵ� �Լ� 
int is_leaf(TreeNode_ptr T) {
    // 1 1 -> 0
    // 1 0 -> 0
    // 0 1 -> 0
    // 0 0 -> 1 
    return !(T->left) && !(T->right);
}
typedef struct {
    char ch;
    int freq;
    int* codes;
    int codes_stop;
}PrintArray;
PrintArray* parray;
// ������ �ڵ尡 ��� �迭�� parray ����ü�� ���� 
void print_codes_array(int codes[], int stop, int freq, char ch) {
    static int i = 0;
    printf("[%d] %c(freq:%d,stop:%d):", i, ch, freq, stop);
    for (int j = 0; j < stop; j++)
        printf("%d", codes[j]);
    printf("\n");
    parray[i].ch = ch;
    parray[i].freq = freq;
    parray[i].codes_stop = stop;
    for (int k = 0; k < parray[i].codes_stop; k++)
        (parray[i].codes)[k] = codes[k];
    i++;
}
// ������ �ڵ尡 ��� �迭 ���� 
void make_codes_array(TreeNode_ptr T, int codes[], int idx) {
    // ���� �ڽ��� ������ 1 ����, ��ȯ ȣ�� 
    if (T->left) {
        codes[idx] = 1;
        make_codes_array(T->left, codes, idx + 1);
    }
    // ������ �ڽ��� ������ 0 ����, ��ȯ ȣ��
    if (T->right) {
        codes[idx] = 0;
        make_codes_array(T->right, codes, idx + 1);
    }
    // ���� ���� ������ �ڵ� �迭�� ����ü�� ���� 
    if (is_leaf(T)) {
        print_codes_array(codes, idx, T->weight, T->ch);
    }
}
// ������ ���� �̿��� ������ Ʈ�� ���� 
// ������ ��: ������ Ʈ�� ������ ���� �� 
// ���� Ʈ��: ������ ���� ����(element)�� ���� ������ Ʈ�� 
// ������ Ʈ��: ������ �ڵ� ������ ���� Ʈ�� (���� ���� Ʈ��)
TreeNode_ptr huffman_tree; // ������ Ʈ�� 
void make_huffman_heap(int freq[], char ch_list[], int n) {
    int i;
    TreeNode_ptr hnode; // ���� Ʈ�� ���
    HeapType_ptr h; // �� 
    element e, e1, e2; // e: ���� ������ �� ����, e1,e2: ���� �� ���  
    int codes[100];
    h = Heap_init();
    // ������ �� �ʱ� ���� 
    for (i = 0; i < n; i++) {
        hnode = make_Htree(NULL, NULL); // ���� Ʈ�� ��� ���� 
        e.ch = hnode->ch = ch_list[i]; // �ʵ忡 ���� �ο� 
        e.key = hnode->weight = freq[i]; // �ʵ忡 ������ �󵵼� �ο� 
        e.htree = hnode; // �ʵ忡 ���� Ʈ���� �ּ� �ο� 
        insert_min_heap(h, e);
    }
    // ������ Ʈ�� ���� 
    for (i = 1; i < n; i++) {
        // �ּҰ��� ������ �� ���� ��带 ����
        e1 = delete_min_heap(h);
        e2 = delete_min_heap(h);
        // �� ���� ��� ���ļ� �� ���� Ʈ�� ���� 
        hnode = make_Htree(e1.htree, e2.htree);
        // ������ ���� �� ���� ���� (�� ���� ��� Ű�� ����)
        e.key = hnode->weight = e1.key + e2.key;
        e.htree = hnode;
        insert_min_heap(h, e);
    }
    e = delete_min_heap(h);
    huffman_tree = e.htree; // ������ Ʈ�� (���� ���� Ʈ��)
    // ������ Ʈ���� �ڵ� ���� (�� ���) 
    make_codes_array(huffman_tree, codes, 0);
}
int geti;
char get_symbol(char string[]) {
    return string[geti++];
}
char* string = NULL;
int input_processing(char* ch_list, int* freq) {
    // �ҹ��� ���ĺ� ó�� ����Ʈ ���� 
    int alphabet = 26;
    char* alphabet_list = (char*)malloc(sizeof(char) * alphabet);
    int* alphabet_freq = (int*)calloc(alphabet, sizeof(int));
    for (int i = 0; i < alphabet; i++)
        alphabet_list[i] = 'a' + i;
    // ���ڿ� ó�� ����Ʈ ���� 
    string = (char*)malloc(sizeof(char) * MAX);
    gets(string);
    // �Էµ� ���ĺ� �󵵼� ���� 
    char tmp = 0;
    int idx = 0;
    geti = 0;
    while ((tmp = get_symbol(string)) != 0) {
        if ('a' <= tmp && tmp <= 'z') {
            idx = tmp - 'a';
            alphabet_freq[idx]++;
        }
    }
    // �󵵼��� 0�� ���ĺ��� ���� 
    int num = 0;
    for (int i = 0; i < alphabet; i++) {
        if (alphabet_freq[i] > 0) {
            ch_list[num] = alphabet_list[i];
            freq[num++] = alphabet_freq[i];
        }
    }
    // �迭 ���
    for (int i = 0; i < num; i++) {
        printf("%c:%d\n", ch_list[i], freq[i]);
    }
    return num;
}
int main(void) {
    // ����(�ҹ���) ó�� ������ ��ȣȭ 
    // �Է��� ���� �迭 
    char* ch_list = (char*)malloc(sizeof(char) * MAX);
    // �Է��� ������ �� �� �迭
    int* freq = (int*)malloc(sizeof(int) * MAX);
    // �Է¹޾Ƽ� �迭�� ����, ������ ���� ��ȯ 
    int num = input_processing(ch_list, freq);
    // ����� ���� ����ü �迭 ���� 
    parray = (PrintArray*)malloc(sizeof(PrintArray) * MAX);
    for (int i = 0; i < num; i++) {
        parray[i].codes = (int*)malloc(sizeof(PrintArray) * MAX);
    }
    // ���ĺ��� ���� ȣ���� �ڵ���� ����ü�� ���� 
    make_huffman_heap(freq, ch_list, num);

    // ȣ���� �ڵ���� �Է��� ���ڿ� �°� �����ϱ� 
    int tmp;
    geti = 0;
    char* huffman_encode = (char*)malloc(sizeof(char));
    int huffman_encode_size = 0;
    while ((tmp = get_symbol(string)) != 0) {
        for (int i = 0; i < num; i++) {
            if (parray[i].ch == tmp) {
                for (int k = 0; k < parray[i].codes_stop; k++) {
                    huffman_encode[huffman_encode_size++] = (parray[i].codes)[k];
                    realloc(huffman_encode, sizeof(char) + huffman_encode_size);
                }
                break;
            }
        }
    }

    // ȣ���� ���ڵ� ��� ���� 
    printf("\n");
    for (int i = 0; i < huffman_encode_size; i++)
        printf("%d", huffman_encode[i]);
    printf("\n");
    // ������ Ʈ�� �̿��ؼ� ���ڵ��ϱ� 
    TreeNode* temp = huffman_tree;
    for (int i = 0; i < huffman_encode_size; i++) {
        if (huffman_encode[i] == 1) {
            temp = temp->left;
            if ((temp->left == NULL) && (temp->right == NULL)) {
                printf("%c", temp->ch);
                temp = huffman_tree;
            }
        }
        else {
            temp = temp->right;
            if ((temp->left == NULL) && (temp->right == NULL)) {
                printf("%c", temp->ch);
                temp = huffman_tree;
            }
        }
    }
    printf("\n");

    return 0;
}