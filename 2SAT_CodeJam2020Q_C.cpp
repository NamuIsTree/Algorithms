#include <algorithm>
#include <memory.h>
#include <functional>
#include <cstdio>
#include <vector>
#include <stack>
using namespace std;

struct act {
	int s, e;
};

int n, m, cnt, scc, dfsn[4001], SN, sn[4001];
vector<int> adj[4001];
bool finished[4001];
stack<int> s;

inline int neg(int x) { return x % 2 ? x - 1 : x + 1; }
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

void init() {
	cnt = scc = SN = 0;
	for (int i = 0; i < 4001; i++)
		adj[i].clear();
	memset(dfsn, 0, sizeof(dfsn));
	memset(sn, 0, sizeof(sn));
	memset(finished, false, sizeof(finished));
}

void add(int x, int y) {
	adj[x].push_back(y);
}

void XOR(int a, int b) {
	add(neg(a), b);
	add(neg(b), a);
	add(a, neg(b));
	add(b, neg(a));
}

bool isIntersect(act a, act b) {
	if (a.s > b.s)
		swap(a, b);
	return a.e > b.s;
}

#define C(i) 2 * i
#define J(i) 2 * n + 2 * i

void solve(int Case) {
	init();
	vector<act> Acts;
	scanf("%d", &n);
	Acts.resize(n);
	for (int i = 0; i < n; i++)
		scanf("%d %d", &Acts[i].s, &Acts[i].e);
	
	for (int i = 0; i < n; i++) {
		XOR(C(i), neg(C(i)));
		XOR(J(i), neg(J(i)));
		XOR(C(i), J(i));
	}

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (isIntersect(Acts[i], Acts[j])) {
				XOR(C(i), C(j));
				XOR(J(i), J(j));
			}
		}
	}
	
	for (int i = 0; i < 4 * n; i++)
		if (dfsn[i] == 0) GetSCC(i);

	bool flag = true;
	for (int i = 0; i < 2 * n; i++) {
		if (sn[2 * i] == sn[2 * i + 1]) {
			flag = false;
			break;
		}
	}

	printf("Case #%d: ", Case);
	if (!flag) {
		printf("IMPOSSIBLE\n");
		return;
	}

	for (int i = 0; i < n; i++) 
		printf(sn[C(i)] < sn[neg(C(i))] ? "C" : "J");
	
	printf("\n");
}

int main() {
	int t;
	scanf("%d", &t);
	for (int i = 1; i <= t; i++)
		solve(i);
	return 0;
}
