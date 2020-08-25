#include <bits/stdc++.h>
using namespace std;
const int MAX = 2e3 + 1;

struct BipartiteMatching {
	int n, match; 	    // index : 1 ~
	int A[MAX], B[MAX]; // A : left side nodes, B : right side nodes.
			    // indices of A : 1 ~ Asize, B : Asize + 1 ~ Asize + Bsize. 
	bool v[MAX];
	vector<int> adj[MAX];

	bool dfs(int i) {
		v[i] = true;
		for (int j : adj[i]) {
			if (B[j] == -1 || (!v[B[j]] && dfs(B[j]))) {
				A[i] = j, B[j] = i;
				return true;
			}
		}
		return false;
	}

	void execute() {
		match = 0;
		memset(A, -1, sizeof(A));
		memset(B, -1, sizeof(B));
		for (int i = 1; i <= n; i++) {
			if (A[i] == -1) {
				memset(v, false, sizeof(v));
				if (dfs(i)) match++;
			}
		}
	}

} BM;


int n, m; // Ã¥ÀÌ n±Ç, »ç¶÷ÀÌ m¸í

void build() {
	scanf("%d %d", &n, &m);
	BM.n = m;

	for (int i = 0; i < m; i++) {
		int s, e;
		scanf("%d %d", &s, &e);
		for (int j = s; j <= e; j++)
			BM.adj[i].push_back(m + j);
	}
}

void solve() {
	BM.execute();
	printf("%d\n", BM.match);
}

void init() {
	for (int i = 1; i <= m; i++)
		BM.adj[i].clear();
}

int main() {
	int t;
	scanf("%d", &t);
	while (t--) {
		build();
		solve();
		init();
	}
	return 0;
}
