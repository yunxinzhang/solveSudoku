#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include<windows.h>
using namespace std;
bool operator==(const pair<int, int> &lhs, const pair<int, int> &rhs)
{
	return lhs.first == rhs.first  && lhs.second == rhs.second;
}

class Sudoku {
public:
	int matrix[9][9];
	map<pair<int, int>, set<int>> candidate;
	Sudoku(const Sudoku &ss) {
		candidate = ss.candidate;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				matrix[i][j] = ss.matrix[i][j];
			}
		}
	}
	Sudoku(string path) {
		ifstream fin("matrix.txt");
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				fin >> matrix[i][j];
			}
		}
		fin.close();
		init_candidate();
	}
	void print() {
		cout << "-------------" << endl;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				cout << matrix[i][j] << "\t";
			}
			cout << endl;
		}
		//Sleep(3);
	}
	void init_candidate() {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (matrix[i][j] == 0) {
					set<int> s;
					for (int k = 1; k <= 9; ++k)s.insert(k);
					candidate.insert(pair<pair<int, int>, set<int>>(pair<int, int>(i, j), s));
				}
			}
		}
	}
	bool win() {
		if (candidate.size() == 0)return true;
		bool flag = true;
		for (auto it : candidate) {
			if (it.second.size() > 1)return false;
		}
		return true;
	}
	void check(int x,int y) {
		
		// 行
		set<int> row;
		for (int i = 0; i < 9; ++i) {
			if (matrix[x][i] != 0) {
				candidate.at(pair<int, int>(x, y)).erase(matrix[x][i]);
			}
			if (matrix[x][i] == 0 && !(y == i)) {
				for (auto it : candidate.at(pair<int, int>(x, i)))
					row.insert(it);
			}
		}
		for (auto it : candidate.at(pair<int, int>(x, y))) {
			if (row.find(it) == row.end()) {
				candidate.at(pair<int, int>(x, y)).clear();
				candidate.at(pair<int, int>(x, y)).insert(it);
				return;
			}
		}
		
		// 列
		set<int> col;
		for (int i = 0; i < 9; ++i) {
			if (matrix[i][y] != 0) {
				candidate.at(pair<int, int>(x, y)).erase(matrix[i][y]);
			}
			if (matrix[i][y] == 0 && !(x == i)) {
				for (auto it : candidate.at(pair<int, int>(i, y)))
					col.insert(it);
			}
		}
		/*cout << "col" << endl;
		for (auto it : col) {
			cout << it << "\t";
		}
		cout << endl;*/
		for (auto it : candidate.at(pair<int, int>(x, y))) {
			if (col.find(it) == col.end()) {
				candidate.at(pair<int, int>(x, y)).clear();
				candidate.at(pair<int, int>(x, y)).insert(it);
				return;
			}
		}
		/*cout << "========" << endl;
		for (auto it : candidate.at(pair<int, int>(xx, yy))) {
			cout << it << "\t";
		}
		cout << endl;*/
		// 块
		int bigrow = x / 3;
		int bigcol = y / 3;
		set<int> block;
		for (int i = bigrow * 3; i < bigrow * 3 + 3; ++i) {
			for (int j = bigcol * 3; j < bigcol * 3 + 3; ++j) {
				if(matrix[i][j] != 0)
					candidate.at(pair<int, int>(x, y)).erase(matrix[i][j]);
				if (matrix[i][j] == 0 && !(x==i&&y==j)) {
					for (auto it : candidate.at(pair<int, int>(i, j)))
						block.insert(it);
				}
			}
		}
		for (auto it : candidate.at(pair<int, int>(x, y))) {
			if (block.find(it) == block.end()) {
				candidate.at(pair<int, int>(x, y)).clear();
				candidate.at(pair<int, int>(x, y)).insert(it);
				return;
			}
		}
		/*cout << "========" << endl;
		for (auto it : candidate.at(pair<int, int>(xx, yy))) {
			cout << it << "\t";
		}
		cout << endl;*/
		// 对角
		/*set<int> diag;
		if (x == y) {
			for (int i = 0; i < 9; ++i) {
				if (matrix[i][i] != 0)
					candidate.at(pair<int, int>(x, y)).erase(matrix[i][i]);
				if (matrix[i][i] == 0 && !(x == i)) {
					for (auto it : candidate.at(pair<int, int>(i, i)))
						diag.insert(it);
				}
			}
			for (auto it : candidate.at(pair<int, int>(x, y))) {
				if (diag.find(it) == diag.end()) {
					candidate.at(pair<int, int>(x, y)).clear();
					candidate.at(pair<int, int>(x, y)).insert(it);
					return;
				}
			}
		}
		
		cout << "========" << endl;
		for (auto it : candidate.at(pair<int, int>(xx, yy))) {
			cout << it << "\t";
		}*/
		//cout << endl;
	}
	// candidate中有set的size=1时，有空确定。确定后会引发其它反应
	bool update() {
		bool flag = false;
		set<pair<int, int>> temp;
		for (auto it = candidate.begin(); it != candidate.end(); ++it) {
			/*cout << it->first.first << " , " << it->first.second << endl;
			for (auto init : it->second) {
				cout << init << "\t";
			}
			cout << endl;*/
			if (it->second.size() == 1) {
				matrix[it->first.first][it->first.second] = *it->second.begin();
				// 删除无法遍历
				//candidate.erase(pair<int, int>(it.first.first, it.first.second));
				temp.insert(pair<int, int>(it->first.first, it->first.second));
				flag = true;
			}
		}
		for (auto it : temp) {
			candidate.erase(it);
		}
		//cout << "update" << endl;
		//cout << flag << endl;
		//print();
		//Sleep(2000);
		//check(7, 8);
		return flag;
	}
	void reset() {
		do {
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if(matrix[i][j]==0)
						check(i, j);
				}
			}
		} while (update());
		
		
	}
	bool isOK() {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (matrix[i][j] == 0) return false;
				if (matrix[i][j] != 0) {
					if (!isOK(i, j))return false;
				}
			}
		}
		return true;
	}
	bool isOK(int x ,int y) {
		for (int i = 0; i < 9; ++i) {
			if (matrix[x][i] == matrix[x][y] && i != y)return false;
		}
		for (int i = 0; i < 9; ++i) {
			if (matrix[i][y] == matrix[x][y] && i != x)return false;
		}
		int r = x / 3;
		int c = y / 3;
		for (int i = r * 3; i < r * 3 + 3; ++i) {
			for (int j = c * 3; j < c * 3 + 3; ++j) {
				if (matrix[i][j] == matrix[x][y] && !(x == i&&y == j))return false;
			}
		}
		return true;
	}
};
bool solve(Sudoku s) {
	if (s.win() && s.isOK()) {
		cout << "result:" << endl;
		s.print();
		return true;
	}
	for (auto it : s.candidate) {
		for (auto init : it.second) {
			//cout <<"size"<< it.second.size() << endl;
			//for (auto k : it.second)cout << k; cout << endl;
			Sudoku news(s);
			news.matrix[it.first.first][it.first.second] = init;
			//cout << "new matrix" << endl;
			//cout << it.first.first << "," << it.first.second << endl;
			//cout << init;
			news.candidate.erase(pair<int, int>(it.first.first, it.first.second));
			news.reset();
			if (solve(news)) {
				return true;
			}
		}
	}
	//cout << "cant solve" << endl;
	return false;// 非正常退出
}
int main() {
	Sudoku sdk("matrix.txt");
	sdk.print();
	//sdk.check(0, 3);
	/*for (auto it : sdk.candidate.at(pair<int, int>(0, 3))) {
		cout << it << "\t";
	}*/
	sdk.reset();
	solve(sdk);
	system("pause");
}