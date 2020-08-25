#include <functional>
#include <algorithm>
#include <cstdio>
#include <set>
using namespace std;

// Copied from koosaga's team note.
using line_t = long long;
const line_t is_query = -1e18;

struct Line {
	line_t m, b;
	mutable function<const Line* ()> succ;
	bool operator < (const Line& rhs) const {
		if (rhs.b != is_query) return m < rhs.m;
		const Line* s = succ();
		if (!s) return 0;
		line_t x = rhs.m;
		return b - s->b < (s->m - m)* x;
	}
};


struct HullDynamic : public multiset<Line> {
	bool bad(iterator y) {
		auto z = next(y);
		if (y == begin()) {
			if (z == end()) return 0;
			return y->m == z->m && y->b <= z->b;
		}
		auto x = prev(y);
		if (z == end()) return y->m == x->m && y->b <= x->b;
		return (x->b - y->b) * (z->m - y->m) >= (y->b - z->b) * (y->m - x->m);
	}

	void insert_line(line_t m, line_t b) {
		auto y = insert({ m,b });
		y->succ = [=] {return next(y) == end() ? 0 : &*next(y); };
		if (bad(y)) { erase(y); return; }
		while (next(y) != end() && bad(next(y))) erase(next(y));
		while (y != begin() && bad(prev(y))) erase(prev(y));
	}

	line_t query(line_t x) {
		auto l = *lower_bound({ x, is_query });
		return l.m * x + l.b;
	}
} CHT;

int n;
void solve() {
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		int q;
		scanf("%d", &q);
		if (q == 1) {
			long long a, b;
			scanf("%lld %lld", &a, &b);
			CHT.insert_line(a, b);
		}
		else {
			long long x;
			scanf("%lld", &x);
			printf("%lld\n", CHT.query(x));
		}
	}
}

int main() {
	solve();
}