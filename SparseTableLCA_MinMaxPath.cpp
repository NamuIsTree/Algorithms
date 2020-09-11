#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
const int MAX = 1e5 + 1;
const int lgM = 18; // log2(MAX) + 1

struct SparseTableLCA {
	int n, root = 0;
	int p[MAX][lgM] = { 0 }, depth[MAX] = { 0 }; // p[i][j] : i번째 노드의 2^j 번째 조상
	int mn[MAX][lgM], mx[MAX][lgM];
	bool v[MAX] = { false };
	vector<pii> adj[MAX];

	void add_edge(int u, int v, int c) {
		adj[u].push_back({ v,c });
		adj[v].push_back({ u,c });
	}

	void dfs() {
		stack<int> s;
		s.push(root);
		while (!s.empty()) {
			int now = s.top();
			s.pop();
			if (v[now]) continue;
			v[now] = true;
			for (pii i : adj[now]) {
				int next = i.first, c = i.second;
				if (!v[next]) {
					depth[next] = depth[now] + 1;
					p[next][0] = now;
					mn[next][0] = mx[next][0] = c;
					s.push(next);
				}
			}
		}
	}

	void construct() {
		dfs();
		for (int d = 1; d < lgM; d++)
			for (int i = 1; i <= n; i++) {
				p[i][d] = p[p[i][d - 1]][d - 1];
				mn[i][d] = min(mn[i][d - 1], mn[p[i][d - 1]][d - 1]);
				mx[i][d] = max(mx[i][d - 1], mx[p[i][d - 1]][d - 1]);
			}
	}

	pii find(int u, int v) {
		int N = 1e9, X = -1; // min, max;
		if (depth[u] < depth[v]) swap(u, v);
		int d = depth[u] - depth[v];
		for (int i = 0; d; i++) {
			if (d & 1) N = min(N, mn[u][i]), X = max(X, mx[u][i]), u = p[u][i];
			d >>= 1;
		}
		if (u == v) return { N,X };
		for (int i = lgM - 1; i >= 0; i--) {
			if (p[v][i] != p[u][i]) {
				N = min(N, min(mn[v][i], mn[u][i]));
				X = max(X, max(mx[v][i], mx[u][i]));
				v = p[v][i], u = p[u][i];
			}
		}
		N = min(N, min(mn[v][0], mn[u][0])), X = max(X, max(mx[v][0], mx[u][0]));
		v = p[v][0];
		return { N,X };
	}

} LCA;

void build() {
	scanf("%d", &LCA.n);
	for (int i = 1, u, v, c; i < LCA.n; i++) {
		scanf("%d %d %d", &u, &v, &c);
		LCA.add_edge(u, v, c);
	}
	LCA.root = 1;
	LCA.construct();
}

void solve() {
	int m; scanf("%d", &m);
	while (m--) {
		int u, v; scanf("%d %d", &u, &v);
		pii ans = LCA.find(u, v);
		printf("%d %d\n", ans.first, ans.second);
	}
}

int main() {
	build();
	solve();
	return 0;
}
