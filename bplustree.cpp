#include <iostream>
#include <fstream>
#include <string>
#include "bplustree.h"
using namespace std;

Bpt::Bpt(string _indexName)
:root(NULL),rootID(0),num(0),indexName(_indexName){
	file.open(indexName.c_str(),
		ios_base::in|ios_base::out|ios_base::binary|ios_base::trunc);
	if(!isEmpty()){
		file.seekg(file.beg);
		file.read((char*)&rootID,sizeof(int));
		file.read((char*)&num,sizeof(int));
		root = new Node();
		readNode(root,rootID);
	}
}

Bpt::Node::Node()
:id(0),n(0),leaf(0){
	memset(key,0,sizeof(int)*2*HALFORDER);
	memset(child,0,sizeof(int)*2*HALFORDER);
}

Bpt::Node::Node(unsigned int _id,unsigned int _n,int _leaf)
:id(_id),n(_n),leaf(_leaf){
	memset(key,0,sizeof(int)*2*HALFORDER);
	memset(child,0,sizeof(int)*2*HALFORDER);
}

Bpt::Node::Node(const Node& _that)
:id(_that.id),n(_that.n),leaf(_that.leaf){
	memcpy(key,_that.key,sizeof(_that.key));
	memcpy(child,_that.child,sizeof(_that.child));
}

void Bpt::insert(int key){
	if(isEmpty()){
		startNewTree(key);
	}
	else{
		insertIntoLeaf(key);
	}
}

void Bpt::startNewTree(int key){
	root = new Node(1,1,1);
	root -> key[0] = key;
	rootID = 1;
	num = 1;
	writeNode(root,1);
	cout<<"Succeed to start a new tree"<<endl;
}

int Bpt::find(int key){
	if(root==NULL){
		return -1;
	}

	Node* tmp = root;
	int id;

	while(!tmp->leaf){
		int i = tmp->n;
		for(;tmp->key[i-1] > key && i > 0;i--);
		
		//Need to check
		if(i < 0){
			cout<<"Can't find it"<<endl;
			return -1;
		}

		int id = tmp->child[i];
		readNode(tmp,id);
	}

	int i = tmp->n;
	for(;tmp->key[i-1] != key && i > 0; i--);

	if(i<=0){
		cout<<"Can't find it"<<endl;
		return -1;
	}
	else{
		cout<<"Find it!"<<endl;
		return id;
	}
}

void Bpt::remove(int key){
	return;
}

void Bpt::insertIntoLeaf(int _key){
	int curId = find(_key);
	if(curId != -1){
		return;
	}
	Node* tmp = new Node();
	readNode(tmp,curId);

	if(tmp->n < 2*HALFORDER){
		int i = keyPosInNode(tmp,_key);
		for(int j = tmp->n -1;j>i;j--){
			tmp->key[j] = tmp->key[j-1];
		}
		tmp->key[i] = _key;
		num++;
		(tmp->n)++;
		writeNode(tmp,curId);
		cout<<"Succeed to insert"<<endl;
	}
	else{
		return;
	}
}

void Bpt::insertIntoParent(int key){
	return;
}

bool Bpt::isEmpty(){
	if(file.beg==file.end-2){
		return true;
	}
	else{
		return false;
	}
}

int Bpt::keyPosInNode(Node* _node,int _key){
	int i = _node->n -1;
	for(;_node->key[i] > _key;i--);
	return i;
}

void Bpt::readNode(Node* _node,int _id){
	file.seekg(sizeof(Node)*(_id-1)+2*sizeof(int));
	file.read((char*)_node,sizeof(Node));
	file.seekg(file.beg);
	file.read((char*)&rootID,sizeof(int));
	file.read((char*)&num,sizeof(int));
}

void Bpt::writeNode(Node* _node,int _id){
	file.seekp(sizeof(Node)*(_id-1)+2*sizeof(int));
	file.write((char*)_node,sizeof(Node));
	file.seekp(file.beg);
	file.write((char*)&rootID,sizeof(int));
	file.write((char*)&num,sizeof(int));
	file.close();
	file.open(indexName.c_str(),
		ios_base::in|ios_base::out|ios_base::binary);
}

int main(){
	Bpt tree("index.txt");
	tree.insert(6);
	tree.find(5);
	return 0;
}