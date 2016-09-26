#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <unordered_map>

using namespace std;

struct Haffman_Node{
	int ch, times;
	Haffman_Node *son[2], *fa;
	Haffman_Node(int _ch, int _times, Haffman_Node *ls = NULL, Haffman_Node *rs = NULL) : ch(_ch), times(_times) {
		son[0] = ls; son[1] = rs; fa = NULL;
	}
};
struct cmp{
	bool operator ()(Haffman_Node *A, Haffman_Node *B){
		return (A -> times) > (B -> times);
	}
};

int ch_num[260];
Haffman_Node* to_Node[260];
priority_queue<Haffman_Node, vector<Haffman_Node*>, cmp> Q;
string code[260];
char *buffer;
int buffer_size;
string buf;

void Pre_work(){
	buffer = new char[200000001];
}

void Getcode(Haffman_Node *t, int id){
	if (t == NULL || t -> fa == NULL) return;
	Getcode(t -> fa, id);
	code[id] += (t == t -> fa -> son[0] ? "0" : "1");
}

void Build_Haffman(const char* argv1){

	ifstream fin(argv1, ios :: binary);
	int char_num = 0;
	for (int ch; (ch = fin.get()) != EOF;){
		++ch_num[ch];
		++char_num;
	}
	fin.close();
	for (int i = 0; i < 256; i++) if(ch_num[i] > 0){
		Haffman_Node *p = new Haffman_Node(i, ch_num[i]);
		to_Node[i] = p;
		Q.push(p);
	}
	while (Q.size() > 1){
		Haffman_Node *p1 = Q.top(); Q.pop();
		Haffman_Node *p2 = Q.top(); Q.pop();
		Haffman_Node *tmp = new Haffman_Node(0, p1 -> times + p2 -> times, p1, p2);
		p1->fa = p2->fa = tmp;
		Q.push(tmp);
	}

	for (int i = 0; i < 256; i++)
		if (ch_num[i] > 0)
			Getcode(to_Node[i], i);

	for (int i = 0; i < 256; i++){
		int len = code[i].size();
		buffer[buffer_size++] = len;
		for (int j = 0; j < len; j++){
			buffer[buffer_size++] = code[i][j];
		}
	}
}

void Compress(const char *argv1){
	ifstream fin(argv1, ios :: binary);
	string output_name = "";
	int len = strlen(argv1);
	for (int i = 0; i < len; i++)
		output_name += argv1[i];
	output_name += ".lcp";
	ofstream fout(output_name.c_str(), ios :: binary);	
	for (int ch; (ch = fin.get()) != EOF;)
		buf += code[ch];
	int buf_size = buf.size();

	int i = 0, j = 0;
	for (i = 0, j = 0; i + 8 <= buf_size; i += 8, ++buffer_size){
		for (j = i; j < i + 8; j++){
			if (buf[j] == '1'){
				buffer[buffer_size] |= (1 << (7 - (j - i)));
			}
		}
	}
	if (i - 8 != buf_size || i == 0){
		int cnt = 0;
		for (; j < buf_size; j++){
			++cnt;
			if (buf[j] == '1')
				buffer[buffer_size] |= 1 << (buf_size - j - 1);
		}
		++buffer_size;

		buffer[buffer_size++] = cnt;
		buffer[buffer_size++] = 127;
	}
	else{
		buffer[buffer_size++] = 0;
		buffer[buffer_size++] = 126;
	}
	//fout.write(buffer, buffer_size);
	for (i = 0; i < buffer_size; i++)
		fout.write((char*)(&buffer[i]), sizeof(buffer[i]));
	fin.close();
	fout.close();
}

int main(int argc, char* argv[]){
	Pre_work();	
	Build_Haffman(argv[1]);
	Compress(argv[1]);
	return 0;
}
