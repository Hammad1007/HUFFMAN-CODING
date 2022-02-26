#include <iostream>
#include <algorithm>
#include <fstream>   // library to handle file handling
#include <queue>     // in built queues
#include <stack>     // in built stack
using namespace std;

// Global Functions used in main 
void Welcome();
void Line();
void End();

class Node {
public:
	int freq;     // frequency of character
	char symb;    // character
	Node* lchild; // left child
	Node* rchild; // right child

	// Default Constructor
	Node() {
		symb = '\0';
		rchild = nullptr;
		lchild = nullptr;
		freq = 0;

	}

	// Paramterised Constructor
	Node(int f, char c, Node* lchild = nullptr, Node* rchild = nullptr ) {
		freq = f;
		symb = c;
	}

	// setter to set the symbol and frequency
	void setfreqsym(int f, int s) {
		freq = f;
		symb = s;
	}

	// gretaer than operator overloaded
	bool operator>(const Node& obj) {
		bool flag = false;
		if (freq > obj.freq) {
			flag = true;
		}
		else {
			flag = false;
		}
		return flag;
	}

	// lesser than operator
	bool operator<(const Node& obj) {
		bool flag = false;
		if (freq < obj.freq) {
			flag = true;
		}
		else {
			flag = false;
		}
		return flag;
	}
	
}; // end of class Node

// Templatised class, Minheap class to determine the order of elements
template <typename t, typename u>
class MinHeap {
	// class of heapitem, contains 2 things, maybe of same data type or different data type
	class HeapItem {
	public:
		t key;
		u value;
	};

private:
	HeapItem* h_arr;  // heap array
	int cap;          // capacity of items an array can hold
	int t_items;      // total items present in the array

	// -----------------------Private Functions to assist the Public Functions-----------------------
	// Doubles the capacity of the heap array
	void Cap_Double() {
		if (this->h_arr == nullptr) {
			this->h_arr = new HeapItem[1];
			this->cap = 1;
		}
		int ncap = 2 * cap;
		HeapItem* narr = new HeapItem[ncap];
		for (int i = 0; i < this->t_items; i++) {
			narr[i] = this->h_arr[i];
		}
		if (this->h_arr != nullptr) {
			delete this->h_arr;
		}
		this->cap = ncap;
		this->h_arr = narr;
	}

	// Moves the value up
	void Move_Up(int index) {
		if (index < 1) { 
			return; 
		}
		int ind = (index - 1) / 2;
		if (this->h_arr[index].key < this->h_arr[ind].key) {
			swap(this->h_arr[index], this->h_arr[ind]);
		}
		Move_Up(ind);
	}

	// Moves the value down
	void Mov_Down(int index) {
		int minindex = -1;
		int rcindex = (index * 2) + 2;
		int lcindex = index * 2 + 1;
		if (minindex == -1) { 
			return; 
		}
		if (lcindex < t_items) {
			if (h_arr[index].key > h_arr[lcindex].key) {
				minindex = lcindex;
			}
		}
		if (rcindex < t_items) {
			if (h_arr[(minindex == -1 ? index : minindex)].key > h_arr[rcindex].key) {
				minindex = rcindex;
			}
		}
		
		swap(h_arr[index], h_arr[minindex]);
		Mov_Down(minindex);
	}

	// Deletes the stuff in array
	void Cleanup() {
		if (this->h_arr != nullptr) {
			delete[]h_arr;
			h_arr = nullptr;
			this->cap = 0;
			this->t_items = 0;
		}
	}
public:

	// Defualt Constructor
	MinHeap() {
		this->h_arr = nullptr;
		this->cap = 0;
		this->t_items = 0;
	}

	// Paramterised Constructor
	MinHeap(int c) {
		this->h_arr = new HeapItem[c];
		this->cap = c;
		this->t_items = 0;
	}

	// returns the total capacity of the array
	int getcap() {
		return cap; 
	}

	// Returns the total size of the array
	int getsize() { 
		return t_items;
	}

	// Checks if the array is empty or not
	bool Empty() const {
		if (t_items == 0) { 
			return true; 
		}
		else {
			return false;
		}
	}

	// Checks if the array is full or not
	bool Full() const {
		if (t_items == cap) { 
			return true; 
		}
		else { 
			return false;
		}
	}

	// Insert function
	// checks if the array has reached its capacity, then doubles it
	// else it inserts the data in the heap
	void Insert(t const dat, u const val) {  // dat corr to key and val corr to value
		if (this->t_items == this->cap) { 
			Cap_Double(); 
		}
		this->h_arr[t_items].key = dat;
		this->h_arr[t_items].value = val;
		Move_Up(t_items);
		t_items++;
	}

	// Deletes the minimum value after extracting it
	void delmin() {
		swap(h_arr[0], h_arr[this->t_items - 1]);
		t_items--;
		Mov_Down(0);
	}

	// Gets the minimum value in the array
	void getmin(u& val) { 
		val = this->h_arr[0].value; 
	}

	// Destructor
	~MinHeap() { 
		Cleanup();
	}

};  // end of class MinHeap

// Beginning of Huffman class
class Huffman {

	// data member is of type Node*
	Node* node;

	// -----------------------Private Functions to assist the Public Functions-----------------------
	// private functions in the class
	void Print(Node* tempnode, string line) const {
		if (tempnode == nullptr) {
			return;
		}
		if (tempnode->symb == '*') {
			Print(tempnode->lchild, line + "0");
			Print(tempnode->rchild, line + "1");
		}
		else if (tempnode->symb != '*') {
			cout << tempnode->symb << " : " << line << endl;
			Print(tempnode->lchild, line + "0");
			Print(tempnode->rchild, line + "1");
		}
		
	}

	// calculates the frequency of characters in a string
	int Freq(string str, char c) {
		int f = 0;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == c) {
				f = f + 1;
			}
		}
		return f;
	}

public:

	// Default Constructor
	Huffman() {
		node = nullptr;
	}

	// Encoding function
	void Encode(string const str) {
		Node* lnode = new Node;
		Node* rnode = new Node;
		MinHeap<int, Node*> mh_obj(str.size());
		string line = str;
		int freq = 0;

		for (int i = 0; i < str.size(); i++) {
			freq = Freq(line, str[i]);
			Node* nnode = new Node(freq, str[i]);
			mh_obj.Insert(freq, nnode);
		}
		while (mh_obj.getsize() != 1) {

			// left child
			mh_obj.getmin(lnode);
			mh_obj.delmin();

			// right child
			mh_obj.getmin(rnode);
			mh_obj.delmin();
			
			Node* tempnode = new Node;
			tempnode->symb = '*';
			tempnode->freq = rnode->freq + lnode->freq;
			tempnode->lchild = lnode;
			tempnode->rchild = rnode;
			mh_obj.Insert(tempnode->freq, tempnode);
		}
		mh_obj.getmin(node);
	}

	// File handling function
	void Filing(string const filename) {
		ifstream file(filename);
		int fnum = 0;
		char ch = '\0';
		Node* lnode = new Node;
		Node* rnode = new Node;
		MinHeap<int, Node*> obj(26);   // 26 is the capacity of the heap array in the class
		if (file.fail() == true) {
			cout << "File could not be opened. Try again\n" << endl;
		}

		for (int i = 0; i < 26; i++) {
			file >> ch;
			file >> fnum;
			Node* NODE = new Node(fnum, ch);
			obj.Insert(fnum, NODE);
		}

		while (obj.getsize() != 1) {
			
			// delete left child
			obj.getmin(lnode);   // gets the minimum value in lnode and then deletes it
			obj.delmin();

			// delete right child
			obj.getmin(rnode);   // gets the minimum value in rnode and then deletes it
			obj.delmin();
			
			// forming a new node and then transfering the frequencies of two minimum nodes in it
			// the symbol of the node is '*'
			Node* tempnode = new Node;
			tempnode->freq = lnode->freq + rnode->freq;
			tempnode->symb = '*';
			tempnode->lchild = lnode;
			tempnode->rchild = rnode;
			obj.Insert(tempnode->freq, tempnode);
		}
		obj.getmin(node);

		file.close();
	}

	// Decode function
	void Decode(string const str) {
		Node* temp = node;
		string line = " ";
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == '1') {
				temp = temp->rchild;
			}
			else {
				temp = temp->lchild;
			}
			if (temp->lchild == nullptr and temp->rchild == nullptr) {
				line = line + temp->symb;
				temp = node;
			}
		}
		cout << "The original string was: " << str << "\n";
		cout << "The decoded message is as follows: " << line << "\n";
		cout << "The length of orignal string was: " << str.size();
		cout << "\n";
		cout << "The length of decoded string is: " << line.size() - 1;
		cout << "\n";

		// 1 is being subtracted because we are consuming one extra space in declaring the variable 'line'
		float size_x = str.size();       // takes the size of the string which is not compressed
		float size_y = line.size() - 1;  // takes the size of the string which is compressed 

		float  ans, var_str;
		var_str = size_y / size_x;       // float value of compressed string size / non compressed string size
		ans = (1 - var_str) * 100;       // compression factor calculated in percentages

		cout << "The compression factor is: " << ans << "%" << endl;
	}

	// Printing the huffman tree
	void print() const {
		Print(node, " ");
	}

	// Destructor of Huffman class
	~Huffman() {
		if (node != nullptr) {
			queue<Node*>hq;
			hq.push(node);
			while (hq.empty() == false) {
				Node* tempnode = hq.front();
				hq.pop();
				if (tempnode->lchild != nullptr) {
					hq.push(tempnode->lchild);
				}
				if (tempnode->rchild != nullptr) {
					hq.push(tempnode->rchild);
				}
				delete tempnode;
			}
		}
		delete[] node;
		node = nullptr;
	}

}; // end of class Huffman

void Line() {
	cout << "\t\t<------------------------------------------------------------------>\n";
}

void End() {
	cout << "\t\t\t<--------------------------------THE END--------------------------------->";
}

// Main driver programme starts here
int main() {
	system("Color F0");

	cout << "\t\t\t--------------------Huffman Object 1--------------------\n\n";

	Huffman h1;   // Huffman class object

	string name = "dat1.txt";   // filename
	
	h1.Filing(name);   // filing function
	cout << "\nOutput: \n";
	h1.print();    // prints the huffman tree
	cout << "\n\n";
	Line();

	cout << "\n\n";
	string word = "00110011";
	
	cout << "\nDecoding the message: \n";
	h1.Decode(word);   // Decode the string above
	cout << "\n\n";
	Line();

	cout << "\n\n";

	cout << "\nEncoding the message: " << "\n";
	string str1 = "Hello world, my name is Hammad Rashid.";
	h1.Encode(str1);  // Encode the string above

	h1.print();

	cout << "\n\n";

	cout << "\t\t\t--------------------Huffman Object 2--------------------\n\n";

	Huffman h2;          // Huffman class object 
	string name1 = "Sample1.txt";   // file name 

	h2.Filing(name1);   // filing function
	cout << "\nOutput: \n";
	h2.print();    // prints the huffman tree
	Line();

	cout << "\n\n";

	string word1 = "0011010111";

	cout << "\nDecoding the message: \n";
	h2.Decode(word1);   // Decode the string above
	Line();

	cout << "\n\n";

	cout << "\nEncoding the message: " << "\n";
	string str2 = "I am a student of FAST NUCES.";
	h2.Encode(str2);  // Encode the string above

	h2.print();

	cout << "\n" << "\n\n";

	cout << "\t\t\t--------------------Huffman Object 3--------------------\n\n";
	
	Huffman h3;          // Huffman class object
	string name2 = "Sample2.txt";      // file name

	h3.Filing(name2);   // filing function
	cout << "\nOutput: \n";
	h3.print();    // prints the huffman tree
	Line();

	cout << "\n\n";

	string word2 = "01010100110011";

	cout << "\nDecoding the message: \n";
	h3.Decode(word2);   // Decode the string above
	Line();

	cout << "\n\n\n";

	cout << "\nEncoding the message: " << "\n";
	string str3 = "I am 20 years old.";
	h3.Encode(str3);  // Encode the string above

	h3.print();

	cout << "\n\n\n";
	End();

	cout << "\n\n";

	system("Pause");  // system pause command
	return 0;

} // Main driver ends here
