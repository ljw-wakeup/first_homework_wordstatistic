#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<io.h>
#include<sys/stat.h>
#include<fstream>
#include<assert.h>
using namespace std;

#define MAXWORD 1024

//hash word operation
typedef struct freNode {
	char * p = NULL;
	unsigned int count = 0;
}freNode;

typedef struct freNode_s {
	char* pA = NULL;
	char* pB = NULL;
	unsigned int count = 0;
};

freNode wordfrequen[10];
freNode_s wordsfrequen[10];


//word comparison and operation
//word comparison


void string_connect(char* pre, char* curr, char* combi) {
	for (; *pre; *(combi++) = *(pre++));
	*combi = ' ';
	for (combi++; *curr; *(combi++) = *(curr++));
	*combi = '\0';

}


int update_word(char* old, char* s) {

	if (strcmp(old, s) > 0) {
		strcpy_s(old, MAXWORD, s);
		return 1;
	}
	return 0;

}


int is_same_word(char* old, char* s) {
	char* oldd = old;
	char* ss = s;
	bool flag = false;
	for (; *old && *s; old++, s++) {
		//¶¼ÊÇ×ÖÄ¸
		if (*old > 64 && *s > 64) {
			if (flag == true) return 0;
			else {
				if ((*old - *s) != 32 && (*old - *s) != -32 && (*old - *s) != 0) return 0;
			}
		}
		//¶¼ÊÇÊý×Ö
		else if (*old < 64 && *s < 64) {
			if (*old != *s) flag = true;
		}
		//ÓÐÊý×ÖÓÐ×ÖÄ¸
		else return 0;
	}
	while (*old) {
		if (*old > 64) return 0;
		old++;
	}
	while (*s) {
		if (*s > 64) return 0;
		s++;
	}
	return 1;

}

//·µ»Ø0£¬²»ÊÇÍ¬Ò»¸ö£»·µ»Ø1 ÊÇÍ¬Ò»¸ö;ÒÑ°üº¬¸üÐÂ





//¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª--



//hash
typedef struct hashNode {
	char* word;
	unsigned int word_number = 0;
	struct hashNode* next = NULL;
}hashNode, *hashNPtr;

typedef struct hashNode_s {
	hashNPtr wordA;
	hashNPtr wordB;
	unsigned int word_number = 0;
	struct hashNode_s* next = NULL;
}hashNode_s, *hashNPtr_s;


#define HASHSIZE 16785407    //size of hash table
#define MUTI 3010349           //the muti in the caculation of hashvalue

hashNPtr wordhash[HASHSIZE] = { NULL };
hashNPtr_s wordshash[HASHSIZE] = { NULL };


//caculate the hashvalue
unsigned long hash_function(char *p) {
	unsigned long h = 0;
	for (; *p; p++) {
		if ((*p > 64) && (*p < 91)) {
			h = MUTI*h + *p;
		}
		else if ((*p > 96) && (*p < 123)) {
			h = MUTI*h + *p - 32;
		}
	}
	return h % HASHSIZE;
}


//insert the word to the hash table
hashNPtr insertword(char* s, hashNPtr *hashp, unsigned int N) {
	unsigned long pos = hash_function(s);
	hashNPtr p;
	for (p = hashp[pos]; p != NULL; p = p->next) {
		if (is_same_word(p->word, s)) {
			update_word(p->word, s);
			(p->word_number)++;
			return p;
		}
	}
	p = new hashNode;
	p->word = new char[N];
	strcpy_s(p->word, N, s);
	p->word_number = 1;
	p->next = hashp[pos];
	hashp[pos] = p;
	return p;
}

void insertwords(hashNPtr pre, hashNPtr curr) {
	int ol;
	unsigned long pos = (hash_function(pre->word) + hash_function(curr->word)) % HASHSIZE;
	hashNPtr_s p;
	for (p = wordshash[pos]; p != NULL; p = p->next) {
		if (is_same_word(p->wordA->word, pre->word) && is_same_word(p->wordB->word, curr->word)) {
			(p->word_number)++;
			return;
		}
	}
	p = new hashNode_s;
	p->wordA = pre;
	p->wordB = curr;
	p->word_number = 1;
	p->next = wordshash[pos];
	wordshash[pos] = p;

}

//hashsort
void hashsort(hashNPtr *hashp, freNode *frep) {
	int i, j, k;
	hashNPtr q;
	for (i = 0; i < HASHSIZE; i++) {
		for (q = hashp[i]; q != NULL; q = q->next) {
			for (j = 0; j < 10; j++) {
				if (q->word_number >frep[j].count) {
					for (k = 8; k >= j; k--) frep[k + 1] = frep[k];
					frep[j].count = q->word_number;
					frep[j].p = q->word;
					break;
				}
			}
		}
	}
	ofstream outfile;
	outfile.open("result.txt", ios::out | ios::app);
	outfile << "the top ten frequency of word:" << endl;
	for (i = 0; i < 10; i++) {
		if (frep[i].p == NULL) return;
		outfile << frep[i].p << " " << frep[i].count << endl;
		//cout << frep[i].p << " " << frep[i].count << endl;
	}
	outfile.close();
}

void hashsort_s(hashNPtr_s *hashp, freNode_s *frep) {
	int i, j, k;
	hashNPtr_s q;
	for (i = 0; i < HASHSIZE; i++) {
		for (q = hashp[i]; q != NULL; q = q->next) {
			for (j = 0; j < 10; j++) {
				if (q->word_number >frep[j].count) {
					for (k = 8; k >= j; k--) frep[k + 1] = frep[k];
					frep[j].count = q->word_number;
					frep[j].pA = q->wordA->word;
					frep[j].pB = q->wordB->word;
					break;
				}
			}
		}
	}
	ofstream outfile;
	outfile.open("result.txt", ios::out | ios::app);
	outfile << "the top ten frequency of phrase:" << endl;
	for (i = 0; i < 10; i++) {
		if (frep[i].pA == NULL || frep[i].pB == NULL) return;
		outfile << frep[i].pA << " " << frep[i].pB << "    " << frep[i].count << endl;
		//cout << frep[i].pA << " " <<  frep[i].pB <<" "<<frep[i].count << endl;
	}
	outfile.close();

}



//¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª



//statistics operation
//char_and_line statistics
//judge a char
short is_char(char c) {
	//Êý×Ö·µ»Ø1
	if (c >= 48 && c <= 57) return 1;
	//×ÖÄ¸·µ»Ø2
	else if (c >= 65 && c <= 90 || c >= 97 && c <= 122) return 2;
	//×Ö·û·µ»Ø3
	else if (c >= 32 && c <= 126) return 3;
	//»»ÐÐ·û·µ»Ø4
	else if (c == '\n') return 4;
	else return 0;
}
//statistic 
void statistics(vector<string> &files) {
	unsigned long charnumber = 0L;
	unsigned long linenumber = 0L;
	unsigned long wordnumber = 0L;
	ifstream infile;
	ofstream outfile;
	hashNPtr curr = NULL;
	hashNPtr pre = NULL;
	char words[MAXWORD] = { 0 };
	//char two_words[MAXWORD * 2] = { 0 };
	char c;
	short ctype;
	short i = 0;
	bool preflag = 0;
	int r = 0;
	for (string a : files) {
		//cout << "a is "<<a << endl;
		infile.open(a.c_str(), ios::in);
		while (!infile.eof()) {
			c = infile.get();
			ctype = is_char(c);
			//Èç¹ûÊÇ×Ö·û
			if (ctype == 1 || ctype == 2 || ctype == 3) {
				charnumber++;
				//Èç¹ûÊÇÊý×Ö
				if (ctype == 1) {
					if (i > 3) {
						words[i] = c;
						i++;
					}
					else {
						memset(words, 0, sizeof(words));
						i = 0;
					}
				}
				//Èç¹ûÊÇ×ÖÄ¸
				else if (ctype == 2) {
					words[i] = c;
					i++;
				}
				//Èç¹ûÊÇ·Ö¸ô·û
				else {
					if (i > 3) {
						wordnumber++;
						curr = insertword(words, wordhash, MAXWORD);
						if (preflag) {
							insertwords(pre, curr);
						}
						pre = curr;
						preflag = 1;

					}
					i = 0;
					memset(words, 0, sizeof(words));
				}

			}
			//Èç¹ûÊÇ»»ÐÐ·û

			else {
				if (ctype == 4)
					linenumber++;
				if (i > 3) {
					wordnumber++;
					curr = insertword(words, wordhash, MAXWORD);
					if (preflag) {
						insertwords(pre, curr);
					}
					pre = curr;
					preflag = 1;

				}
				i = 0;
				memset(words, 0, sizeof(words));
			}
			//µ¥´ÊÍ³¼Æ
		}
		i = 0;
		preflag = 0;
		memset(words, 0, sizeof(words));
		linenumber++;
		infile.close();
	}
	//cout << charnumber << endl;
	//cout << linenumber << endl;
	//cout << wordnumber << endl;
	outfile.open("result.txt", ios::out);
	outfile << "char_number :" << charnumber << endl;
	outfile << "line_number :" << linenumber << endl;
	outfile << "word_number :" << wordnumber << endl;
	outfile.close();
}

//¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª





//¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª--






//folder and files operation
//path is a folder
int getfiles(string path, vector<string> &files) {

	//¿ÕÔò·µ»Ø
	if (path.empty()) {
		cout << "The path is empty!\n";
		return 1;
	}

	//ÎÄ¼þÐÅÏ¢½á¹¹Ìå
	struct _finddata_t fileinfo;
	string p;
	p = path + "\\*";
	//ÎÄ¼þ¾ä±ú
	long hfile = 0L;
	hfile = _findfirst(p.c_str(), &fileinfo);
	if (hfile == -1L)
		cout << "cannot match the folder path\n";
	else {
		do
		{
			//Èç¹ûÊÇÄ¿Â¼£¬µü´ú
			//Èç¹û²»ÊÇÔò¼ÓÈëÁÐ±í
			if (fileinfo.attrib & _A_SUBDIR) {
				if ((strcmp(fileinfo.name, ".") != 0) && (strcmp(fileinfo.name, "..") != 0)) {
					getfiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hfile, &fileinfo) == 0);
	}
	_findclose(hfile);
	return 0;
}

//judge a path :dir return 1; file return 0
bool judgedir(string path) {
	int result;
	struct _stat buf;
	result = _stat(path.c_str(), &buf);
	if (_S_IFDIR & buf.st_mode) {
		//cout << "It is a folder."<<endl;
		return true;
	}
	else if (_S_IFREG & buf.st_mode) {
		//cout << "It is a file" << endl;
		return false;
	}
	else return false;
}




//¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª





int main(int argc, char* argv[]) {

	string filepath(argv[1]);
	vector<string> files;
	if (judgedir(filepath)) {
		getfiles(filepath, files);
	}
	else {
		files.push_back(filepath);
	}
	int size = files.size();
	int i;
	for (i = 0; i < size; i++) {
		//cout << files[i] << endl;
	}
	statistics(files);

	hashsort(wordhash, wordfrequen);
	hashsort_s(wordshash, wordsfrequen);
	return 0;


}

