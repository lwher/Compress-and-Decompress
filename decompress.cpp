#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <unordered_map>

using namespace std;

int pre_char, buffer_size;
unordered_map<int, int>tree;
char *buffer;
string binary[260], buf, code[260];

void Pre_work(){
	buffer = new char[200000001];	
	for(char p = -128; p <= 127; p++){
		unsigned char q = p;
		binary[p + 128] = "";
		for(int i = (1 << 7); i; i >>= 1){
			if(q & i) 
				binary[p + 128] += '1';
			else 
				binary[p + 128] += '0';
		}
		if(p == 127) break;	
	}
}

void Read_tree(const char* argv1){
	ifstream fin(argv1, ios :: binary);
	for (int i = 0; i < 256; i++){
		code[i] = "";
		int len = fin.get();
		pre_char += len + 1;
		for (int j = 0; j < len; j++)
			code[i] += fin.get();
	}
	fin.close();
}

void Build_huffman_tree(){
	int id;	
	for (int i = 0; i < 256; i++){
		int len = code[i].size();
		if (!len) continue;
		id = 1;
		for (int j = 0; j < len; j++){
			if (code[i][j] == '0')
				id <<= 1;
			else
				id = (id << 1 | 1);
		}
		tree[id] = i;
	}
}

void Read_file(const char* argv1){
	ifstream fin(argv1, ios :: binary);
	fin.seekg(0, fin.end);
	buffer_size = static_cast<int>(fin.tellg()) - pre_char;
	fin.seekg(0, fin.beg);
	for (int i = 0; i < 256; i++){
		int len = fin.get();
		for (int j = 0; j < len; j++)
			fin.get();
	}
	fin.read(buffer, buffer_size);	
	fin.close();
	for (int i = 0; i < buffer_size - 3; i++)
		buf += binary[buffer[i] + 128];
	
	if (buffer[buffer_size - 1] == 127){ // not complete
		string tmp = binary[buffer[buffer_size - 3] + 128];
		while (tmp.size() > buffer[buffer_size - 2])
			tmp.erase(tmp.begin());
		buf += tmp;
	}
	else { //complete
		buf += binary[buffer[buffer_size - 3] + 128];
	}
}

void Decompress(const char *argv1){
	string name = "";
	int len = strlen(argv1);
	for (int i = 0; i < len - 4; i++)
		name += argv1[i];
	ofstream fout(name.c_str(), ios :: binary);
	int id = 1; len = buf.size();
	for (int i = 0; i < len; i++){
		if (buf[i] == '0')
			id <<= 1;
		else
			id =(id << 1 | 1);
		if (tree.count(id)){
			fout.put(tree[id]);
			id = 1;
		}
	}
	fout.close();
}

int main(int argc, const char* argv[]){
	Pre_work();
	Read_tree(argv[1]);
	Build_huffman_tree();
	Read_file(argv[1]);
	Decompress(argv[1]);	
	return 0;
}
