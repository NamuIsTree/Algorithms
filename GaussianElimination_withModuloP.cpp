#include <bits/stdc++.h>
using namespace std;
const int MAXN = 410;
const int MAXM = 410;

struct GaussianElimination_ModuloP {
	// Gaussian Elimination with modulo p 
	// x : n by m, y : n by 1 -> augmented matrix : n by m + 1.
	int n, m, p, x[MAXN][MAXM], y[MAXN], a[MAXN][MAXM], ans[MAXN];
	int* inv;

	void init() {
		memset(x, 0, sizeof(x));
		memset(y, 0, sizeof(y));
		memset(a, 0, sizeof(a));
		memset(ans, 0, sizeof(ans));

		// Pre-calculate (i^-1) mod p. 
		// (if p^p is too big, then use the fact that (i^-1) mod p == (i^(p-2)) mod p).
		inv = new int[p];
		inv[0] = 0;
		for (int i = 1; i < p; i++)
			for (int j = 1; j < p; j++)
				if ((i * j) % p == 1) inv[i] = j;
	}

	void build_aug() {
		for (int i = 0; i < n; i++) {
			a[i][m] = y[i];
			for (int j = 0; j < m; j++)
				a[i][j] = x[i][j];
		}
	}

	void execute() {
		vector<int> w(m, -1);
		for (int c = 0, r = 0; c < m && r < n; ++c) {
			int sel = r;
			for (int i = r; i < n; ++i)
				if (abs(a[i][c]) > abs(a[sel][c]))
					sel = i;

			if (a[sel][c] == 0) continue; // Free variable

			for (int i = 0; i <= m; ++i)
				swap(a[sel][i], a[r][i]);
			w[c] = r;

			int t = inv[a[r][c]];
			for (int i = c; i <= m; i++)
				a[r][i] = (a[r][i] * t) % p;

			for (int i = 0; i < n; ++i) {
				if (i == r) continue;
				int t = a[i][c];
				for (int k = c; k <= m; k++)
					a[i][k] = (a[i][k] - a[r][k] * t + p * p) % p;
			}
			++r;
		}

		for (int i = 0; i < m; i++)
			if (w[i] != -1) {
				ans[i] = a[w[i]][m];
			}
	}

} GE;

int n, m;
int b[21][21];

void build() {
	scanf("%d %d %d", &n, &m, &GE.p);
	GE.n = GE.m = n * m;
	GE.init();

	// make y
	for (int i = 0, t; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%d", &b[i][j]);
			GE.y[i * m + j] = GE.p - b[i][j];
			b[i][j]--;
		}
	}
	// make x
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int now = i * m + j;
			for (int ki = 0; ki < n; ki++)
				GE.x[now][ki * m + j] = 1;
			for (int kj = 0; kj < m; kj++)
				GE.x[now][i * m + kj] = 1;
		}
	}

	GE.build_aug();
}

void solve() {
	GE.execute();
	int sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int now = i * m + j, psh = GE.ans[now];
			sum += psh;
			b[i][j] = (b[i][j] + psh) % GE.p;
			for (int ki = 0; ki < n; ki++) {
				if (ki == i) continue;
				b[ki][j] = (b[ki][j] + psh) % GE.p;
			}
			for (int kj = 0; kj < m; kj++) {
				if (kj == j) continue;
				b[i][kj] = (b[i][kj] + psh) % GE.p;
			}
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (b[i][j] != GE.p - 1) {
				printf("-1"); return;
			}
		}

	printf("%d\n", sum);
	for (int i = 0; i < n * m; i++)
		for (int j = 0; j < GE.ans[i]; j++)
			printf("%d ", i + 1);
}

int main() {
	build();
	solve();
	return 0;
}