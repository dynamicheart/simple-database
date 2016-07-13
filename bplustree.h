#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory.h>
#define HALFORDER 4
#define MAXKEY 304
using namespace std;

class Bpt{
public:
	Bpt(string _indexName);
	int find(int key);
	void insert(int key);
	void remove(int key);
	bool isEmpty();
private:
	struct Node{
		unsigned int id;
		unsigned int n;
		int leaf;
		int key[2*HALFORDER];
		unsigned int child[2*HALFORDER+1];

		Node();
		Node(unsigned int _id,unsigned int _n,int _leaf);
		Node(const Node& _that);
	};
	
	Node* root;
	unsigned int rootID;
	unsigned int num;
	string indexName;
	fstream file;

	void startNewTree(int Key);
	void insertIntoLeaf(int Key);
	void insertIntoParent(int Key);
	void split();
	void readNode(Node* _node,int _id);
	void writeNode(Node* _node,int _id);
	int keyPosInNode(Node* _node,int _key);
};