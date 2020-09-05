#include <bits/stdc++.h>
using namespace std;

const int MAX = 5e4 + 1;
const int lgM = 17; // log2(MAX) + 1

struct SparseTableLCA {
	int n, root = 0, inDegree[MAX] = { 0 };
	int p[MAX][lgM] = { 0 }, depth[MAX] = { 0 }; // p[i][j] : i번째 노드의 2^j 번째 조상
	bool v[MAX] = { false };
	vector<int> adj[MAX];

	void add_edge(int u, int v) {
		adj[u].push_back(v);
		adj[v].push_back(u);
		inDegree[u]++, inDegree[v]++;
	}

	void dfs() {
		stack<int> s;
		s.push(root);
		while (!s.empty()) {
			int now = s.top();
			s.pop();
			if (v[now]) continue;
			v[now] = true;
			for (int next : adj[now])
				if (!v[next]) {
					depth[next] = depth[now] + 1;
					p[next][0] = now;
					s.push(next);
				}
		}
	}

	void construct() {
		dfs();
		for (int d = 1; d < lgM; d++)
			for (int i = 1; i <= n; i++)
					p[i][d] = p[p[i][d - 1]][d - 1];
	}

	int find(int u, int v) {
		if (depth[u] < depth[v]) swap(u, v);
		int d = depth[u] - depth[v];
		for (int i = 0; d; i++) {
			if (d & 1) u = p[u][i];
			d >>= 1;
		}
		if (u == v) return u;
		for (int i = lgM - 1; i >= 0; i--) {
			if (p[v][i] != p[u][i])
				v = p[v][i], u = p[u][i];
		}
		return p[v][0];
	}

} LCA;

void build() {
	scanf("%d", &LCA.n);
	for (int i = 1, u, v; i < LCA.n; i++) {
		scanf("%d %d", &u, &v);
		LCA.add_edge(u, v);
	}
	LCA.root = 1;
	LCA.construct();
}

void solve() {
	int m; scanf("%d", &m);
	while (m--) {
		int u, v; scanf("%d %d", &u, &v);
		printf("%d\n", LCA.find(u, v));
	}
}

int main() {
	build();
	solve();
	return 0;
}
