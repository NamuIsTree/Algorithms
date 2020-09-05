#include <stdio.h>
#include <stack>
#include <vector>
#include <algorithm>
#define MAX 10001
using namespace std;

int id, d[MAX];
bool finished[MAX];
vector<int> a[MAX]; //인접리스트
vector<vector<int> > SCC;
stack<int> s;

int dfs(int x) {
	d[x] = ++id;
	s.push(x);

	int parent = d[x];
	for (int i = 0; i < a[x].size(); i++) {
		int y = a[x][i];
		if (d[y] == 0) {
			parent = min(parent, dfs(y));
		}
		else if (!finished[y]) {
			parent = min(parent, d[y]);
		}
	}
  
	if (parent == d[x]) {
		vector<int> scc;

		while (1) {
			int t = s.top();
			s.pop();
			scc.push_back(t);
			finished[t] = true;
			if (t == x) break;
		}
		SCC.push_back(scc);
	}
	return parent;
}

int main() {
	int v, e;
	scanf("%d %d", &v, &e);
	for (int i = 0; i < e; i++) {
		int b, c;
		scanf("%d %d", &b, &c);
		a[b].push_back(c);
	}
	
	for (int i = 1; i <= v; i++) {
		if (d[i] == 0) dfs(i);
	}
	
	for(int i = 0; i < SCC.size(); i++)
		sort(SCC[i].begin(), SCC[i].end());
	sort(SCC.begin(), SCC.end());
	
	printf("%d\n", SCC.size());
	for (int i = 0; i < SCC.size(); i++) {
		for (int j = 0; j < SCC[i].size(); j++) {
			printf("%d ", SCC[i][j]);
		}
		printf("-1\n");
	}
	return 0;
}
