# include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<string>
#include<fstream>
#include <sstream>
#define N_MAX 60

/*this program uses Haughman Process to create a list of codes (Haughman Code) based from an inputted text file. 
 These codes are used to decypher an encprypted message*/
using namespace std;

struct Node {
	int freq;
	char letter;
	int binary;

	Node *right = NULL;
	Node *left = NULL;
	Node *parent = NULL;

}; typedef struct Node Node;

struct Stack_Node
{
	char bin;
	Stack_Node *next = NULL;

}; typedef struct Stack_Node Stack_Node;

struct Code_event
{
	char letter;
	string code;
}; typedef struct Code_event Code_event;

void createPointerArray(string lines1[], Node *events[], int n);
void printArray(Node *node[], int n);
void printNode(Node *node);
void SORTDES(Node *node[], int n);
void sortAlph(Code_event node[], int n);
void swap_Alph(Code_event *a, Code_event *b);
void swap(Node *a, Node *b);
void RegprintArray(Node node[], int n);
void RegprintNode(Node node);
void Map_Code(Node *events[], Code_event codebook[], int event_size);
void Codebook_Display(Code_event codebook[], int event_size);
string DecodeBinary(string bin, Code_event codebook[], int event_size);

void pop_Node();
void push_Node(char data_in);
void print_Stack(Stack_Node *node);

Stack_Node *Top = NULL;

int main()
{
	int count_1 = 0;
	int count_2 = 0;
	int reset;
	int event_size;

	string decode;

	Node *events[N_MAX];
	Node *parents_temp[N_MAX];
	Code_event codebook[N_MAX];

	int count_parent = 0;
	int final_counter = 0;
	bool parent_found = false;
	int  count_same = 0;

	int stop_len;

	Node *small_1 = new Node();
	Node *small_2 = new Node();

	string lines1[N_MAX];
	string lines2[2];

	string filename_1 = "a4-1.txt";
	string filename_2 = "a4-2.txt";

	ifstream myfile_1(filename_1.c_str());
	ifstream myfile_2(filename_2.c_str());


	if (myfile_1.is_open() == true)
	{
		while (getline(myfile_1, lines1[count_1]))
		{
			count_1++;
		}
		myfile_1.close();
	}
	if (myfile_2.is_open() == true)
	{
		while (getline(myfile_2, lines2[count_2]))
		{
			count_2++;
		}
		myfile_1.close();
	}

	event_size = atoi(lines1[0].c_str());

	stop_len = event_size - 1;
	reset = event_size;

	createPointerArray(lines1, events, event_size);

	for (int i = 0; i < 5; i++)
	{
		parents_temp[i] = new Node();
	}

	SORTDES(events, event_size);
	cout << "Sorted in Descending Order:" << endl;
	printArray(events, event_size);

	while (event_size > 1 || final_counter < stop_len)
	{
		small_1 = events[0];
		small_2 = events[0];
		/*checks to for the 2 smallest numbers in the event array */
		for (int i = 0; i < event_size; i++)
		{
			if (events[i]->freq < small_1->freq)
			{
				small_2 = small_1;
				small_1 = events[i];
			}
			else if (events[i]->freq < small_2->freq && events[i]->freq != small_1->freq)
			{
				small_2 = events[i];
			}
		}
		/* checks to see if there's another smallest number in the parent array*/
		for (int i = 0; i < count_parent; i++)
		{
			if (parents_temp[i]->freq < small_1->freq)
			{
				parent_found = true;
				small_2 = small_1;
				small_1 = parents_temp[i];
				count_same++;
			}
			else if (parents_temp[i]->freq < small_2->freq && parents_temp[i]->freq != small_1->freq)
			{
				parent_found = true;
				small_2 = parents_temp[i];
				count_same++;
			}
		}
		/*Creates a new parents node */
		Node *parent = new Node();

		parent->freq = small_1->freq + small_2->freq;
		small_1->parent = parent;
		small_2->parent = parent;

		if (small_1->freq < small_2->freq)
		{
			parent->left = small_1;
			small_1->binary = 0;
			parent->right = small_2;
			small_2->binary = 1;
		}
		else if (small_2->freq > small_1->freq)
		{
			parent->left = small_2;
			small_2->binary = 1;
			parent->right = small_1;
			small_1->binary = 0;
		}
		parents_temp[count_parent] = parent;

		count_parent++;
		/* if one of the smallest numbers are found from
		the parent array, this will influence how much the event array decreases */
		if (parent_found == true)
		{
			if (count_same == 2)
			{
				for (int i = 1; i <= count_parent; i++)
				{
					parents_temp[i - 1] = parents_temp[i];
				}

				count_parent--;

				for (int i = 1; i <= count_parent; i++)
				{
					parents_temp[i - 1] = parents_temp[i];
				}
			}
			else
			{
				for (int i = 1; i <= count_parent; i++)
				{
					parents_temp[i - 1] = parents_temp[i];
				}
			}

			event_size = event_size - 1;
			count_parent--;
			count_same = 0;
		}
		else
		{
			event_size = event_size - 2;
		}
		parent_found = false;
		final_counter++;

	}

	event_size = reset;

	Map_Code(events, codebook, event_size);
	sortAlph(codebook, event_size);
	Codebook_Display(codebook, event_size);
	decode = DecodeBinary(lines2[0], codebook, event_size);

	cout << "Decoded Message: " << decode << endl << endl;
	system("PAUSE");
	return 0;
}

string DecodeBinary(string bin, Code_event codebook[], int event_size)
{
	string temp_2 = "";
	string decode = "";

	cout << endl << "Encoded Message: " << bin << endl;
	cout << endl;
	int pos = 0;
	while (pos < bin.length())
	{
		temp_2 += bin[pos];

		for (int i = 0; i < event_size; i++)
		{
			if (temp_2 == codebook[i].code)
			{
				decode += codebook[i].letter;
				temp_2 = "";
			}
		}
		pos++;
	}
	return decode;
}
void Codebook_Display(Code_event codebook[], int event_size)
{
	cout << "Code book: " << endl << endl;

	for (int i = 0; i < event_size; i++)
	{
		cout << codebook[i].letter << ": " << codebook[i].code << endl;
	}
}
void Map_Code(Node *events[], Code_event codebook[], int event_size)
{
	Node *temp;
	Stack_Node *t;
	string temp_s;

	int j = 0;
	for (int i = event_size - 1; i >= 0; i--)
	{
		temp = events[i];
		temp_s = "";
		while (temp != NULL && temp->parent != NULL)
		{
			if (temp->binary == 0)
			{
				push_Node('0');
			}
			else
			{
				push_Node('1');
			}
			temp = temp->parent;
		}

		t = Top;
		while (t != NULL)
		{
			temp_s = temp_s + t->bin;
			t = t->next;
			pop_Node();
		}
		codebook[j].code = temp_s;
		codebook[j].letter = events[i]->letter;
		j++;
	}
}
void pop_Node()
{
	Stack_Node *temp;

	if (Top == NULL)
	{
		cout << "\nThe stack is empty" << endl << endl;
	}
	temp = Top;
	Top = Top->next;

	delete(temp);
}

void push_Node(char data_in)
{
	Stack_Node *new_add = new Stack_Node();
	new_add->bin = data_in;
	new_add->next = Top;
	Top = new_add;
}

void print_Stack(Stack_Node *node)
{
	if (Top == NULL)
	{
		cout << "\nSTACK IS EMPTY!!!" << endl;
	}
	else if (Top != NULL) {

		Stack_Node *temp;

		cout << "\nTop" << endl;
		for (temp = node; temp != NULL; temp = temp->next)
		{
			cout << temp->bin << endl;
		}
		cout << "\nBottom" << endl << endl;
	}
}


void printArray(Node *node[], int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << node[i]->freq << " " << node[i]->letter << endl;
	}
	cout << endl;
}

void RegprintArray(Node node[], int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << node[i].freq << " " << node[i].letter << endl;
	}
	cout << endl;
}

void printNode(Node *node)
{
	cout << "Node frequency: " << node->freq << " " << node->letter << endl;
	cout << " Node binary value:" << node->binary << endl;

	if (node->parent == NULL)
	{
		cout << "Parent is NULL, parent points to nothing" << endl;
	}
	else
	{
		cout << "parent frequency: " << node->parent->freq << endl;;
	}

	if (node->left == NULL)
	{
		cout << "left is NULL, left points to nothing" << endl;
	}
	else
	{
		cout << "left frequency:" << node->left->freq << endl;

	}

	if (node->right == NULL)
	{
		cout << "right is NULL, right points to nothing" << endl;
	}
	else
	{
		cout << "right frequency:" << node->right->freq << endl;
	}

	cout << endl;
	cout << endl;
}
void RegprintNode(Node node)
{
	cout << "Node frequency: " << node.freq << " " << node.letter << endl;
	cout << " Node binary value:" << node.binary << endl;

	if (node.parent == NULL)
	{
		cout << "Parent is NULL, parent points to nothing" << endl;
	}
	else
	{
		cout << "parent frequency: " << node.parent->freq << endl;;
	}

	if (node.left == NULL)
	{
		cout << "left is NULL, right points to nothing" << endl;
	}
	else
	{
		cout << "left frequency:" << node.left->freq << endl;

	}

	if (node.right == NULL)
	{
		cout << "right is NULL, left points to nothing" << endl;
	}
	else
	{
		cout << "right frequency:" << node.right->freq << endl;
	}

	cout << endl;
	cout << endl;
}
void SORTDES(Node *node[], int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int i = 1; i < n; i++)
		{
			if (node[i]->freq > node[i - 1]->freq)
			{
				swap(node[i], node[i - 1]);
			}
		}
	}
}
void sortAlph(Code_event node[], int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int i = 1; i < n; i++)
		{
			if (node[i].letter < node[i - 1].letter)
			{
				swap_Alph(&node[i], &node[i - 1]);
			}
		}
	}
}

void swap_Alph(Code_event *a, Code_event *b)
{
	Code_event temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
void swap(Node *a, Node *b)
{
	Node temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
void createPointerArray(string lines1[], Node *events[], int n)
{
	string num;
	int new_count = 1;
	int pos = 0;

	for (int i = 0; i < n; i++)
	{
		events[i] = new Node;

		if (lines1[new_count][0] > 96 && lines1[new_count][0] < 123)
		{

			while (lines1[new_count][pos] != 32)
			{
				pos++;
			}
			events[i]->letter = lines1[new_count][0];
			num = lines1[new_count].substr(pos + 1, lines1[new_count].length() - (pos + 1));
			events[i]->freq = atoi(num.c_str());
			pos = 0;
			new_count++;
		}

	}
}
