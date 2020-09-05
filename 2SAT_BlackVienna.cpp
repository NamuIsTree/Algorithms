#include <cstdio>
#include <vector>
#include <stack>
#include <algorithm>
#include <memory.h>
using namespace std;
const int MAX = 52;
typedef pair<int, int> ii;

struct invest {
	int suspect1, suspect2, n;
};

vector<invest> P[2];

int cnt, scc, dfsn[MAX * 2], SN, sn[MAX * 2];
bool finished[MAX * 2];
vector<int> adj[MAX * 2];
stack<int> s;

void build() {
	int m; scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		char u, v; int p, c;
		scanf(" %c%c %d %d", &u, &v, &p, &c);
		P[p - 1].push_back({ u - 'A',v - 'A',c });
	}
}

void init() {
	cnt = scc = SN = 0;
	for (int i = 0; i < 2 * MAX; i++)
		adj[i].clear();
	memset(dfsn, 0, sizeof(dfsn));
	memset(sn, 0, sizeof(sn));
	memset(finished, false, sizeof(finished));
}

int GetSCC(int curr) {
	dfsn[curr] = ++cnt;
	s.push(curr);

	int result = dfsn[curr];
	for (int next : adj[curr]) {
		if (dfsn[next] == 0) result = min(result, GetSCC(next));
		else if (!finished[next]) result = min(result, dfsn[next]);
	}

	if (result == dfsn[curr]) {
		while (true) {
			int t = s.top();
			s.pop();
			finished[t] = true;
			sn[t] = SN;
			if (t == curr) break;
		}
		SN++;
	}
	return result;
}

inline int neg(int x) {
	return x % 2 ? x - 1 : x + 1;
}

void add(int x, int y) {
	adj[x].push_back(y);
}

void forceTofalse(int x) {
	add(x, neg(x));
}

void forceTotrue(int x) {
	forceTofalse(neg(x));
}

void XOR(int a, int b) {
	add(neg(a), b);
	add(neg(b), a);
	add(a, neg(b));
	add(b, neg(a));
}

bool solve(const int bv1, const int bv2, const int bv3) {
	init();

	for (int i = 0; i < 26; i++) {
		int a = 2 * i, b = 52 + 2 * i;
		if (i == bv1 || i == bv2 || i == bv3)
			forceTofalse(a), forceTofalse(b);
		else XOR(a, b);
	}

	for (int p = 0; p < 2; p++) {
		for (invest v : P[p]) {
			int a = 2 * v.suspect1 + p * 52, b = 2 * v.suspect2 + p * 52;

			switch (v.n) {
			case 0:
				forceTofalse(a), forceTofalse(b); break;
			case 1:
				XOR(a, b); break;
			default:
				forceTotrue(a), forceTotrue(b); break;
			}
		}
	}

	for (int i = 0; i < MAX * 2; i++)
		if (dfsn[i] == 0) GetSCC(i);

	for (int i = 0; i < MAX; i++) {
		if (sn[2 * i] == sn[2 * i + 1])
			return false;
	}

	return true;
}


int main() {
	build();
	int ans = 0;
	for (int i = 0; i < 26; i++)
		for (int j = i + 1; j < 26; j++)
			for (int k = j + 1; k < 26; k++)
				if (solve(i, j, k)) {
					ans++;
				}
	printf("%d\n", ans);
}
