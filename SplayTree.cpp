#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int inf = 1e9 + 7;

int arr[1010101];

struct Node {
    Node* l, * r, * p;
    ll v, sz, sum, mx, mn, lazy;
    bool dummy, flip;

    Node(ll _v, Node* p) : v(_v), p(p) {
        l = r = nullptr;
        sum = mx = mn = _v;
        sz = 1, dummy = 0, flip = 0;
    }
    Node(int _v) : Node(_v, nullptr) {}
    Node() : Node(0) {}
    ~Node() {
        if (l) delete l;
        if (r) delete r;
    }
};

struct SplayTree {
    Node* root;
    Node* ptr[1010101]; //node pointer

    void update(Node* x) {
        x->sz = 1;
        x->sum = x->mn = x->mx = x->v;
        if (x->l) {
            x->sz += x->l->sz;
            x->sum += x->l->sum;
            x->mn = min(x->mn, x->l->mn);
            x->mx = max(x->mx, x->l->mx);
        }
        if (x->r) {
            x->sz += x->r->sz;
            x->sum += x->r->sum;
            x->mn = min(x->mn, x->r->mn);
            x->mx = max(x->mx, x->r->mx);
        }
    }

    void push(Node* x) {
        if (!x->flip) return;
        swap(x->l, x->r);
        if (x->l) x->l->flip = !x->l->flip;
        if (x->r) x->r->flip = !x->r->flip;
        x->flip = 0;
    }

    void push_sum(Node* x) {
        x->v += x->lazy;
        if (x->l) {
            x->l->lazy += x->lazy;
            x->l->sum += x->l->sz * x->lazy;
        }
        if (x->r) {
            x->r->lazy += x->lazy;
            x->r->sum += x->r->sz * x->lazy;
        }
        x->lazy = 0;
    }

    Node* gather(int s, int e) { //gather [s, e]
        kth(e + 1);
        auto tmp = root;
        kth(s - 1);
        splay(tmp, root);
        return root->r->l;
    }

    void rotate(Node* x) {
        auto p = x->p;
        Node* y;
        push(p); push(x);
        if (x == p->l) {
            p->l = y = x->r;
            x->r = p;
        }
        else {
            p->r = y = x->l;
            x->l = p;
        }
        x->p = p->p; p->p = x;
        if (y) y->p = p;
        if (x->p) {
            if (p == x->p->l) x->p->l = x;
            else x->p->r = x;
        }
        else root = x;
        update(p); update(x);
    }

    void splay(Node* x, Node* g = nullptr) {
        Node* y;
        while (x->p != g) {
            Node* p = x->p;
            if (p->p == g) {
                rotate(x); break;
            }
            auto pp = p->p;
            if ((p->l == x) == (pp->l == p)) {
                rotate(p); rotate(x);
            }
            else {
                rotate(x); rotate(x);
            }
        }
        if (!g) root = x;
    }

    SplayTree() : root() {
        memset(ptr, 0, sizeof ptr);
    }
    ~SplayTree() {
        if (root) delete root;
    }

    void init(int n) {
        if (root) delete root;
        root = new Node(-inf); //left dummy node
        auto now = root;
        // Init data to arr[i]
        for (int i = 1; i <= n; i++) {
            ptr[i] = now->r = new Node(arr[i], now);
            now = now->r;
        }
        now->r = new Node(inf, now); //right dummy node
        root->dummy = now->r->dummy = 1;
        for (int i = n; i >= 1; i--) update(ptr[i]);
        splay(ptr[n / 2]);
    }

    void flip(int s, int e) {
        Node* x = gather(s, e);
        x->flip = !x->flip;
    }

    void shift(int s, int e, int k) {
        Node* x = gather(s, e);
        if (k >= 0) {
            k %= (e - s + 1);
            if (!k) return;
            flip(s, e); flip(s, s + k - 1); flip(s + k, e);
        }
        else {
            k *= -1;
            k %= (e - s + 1);
            if (!k) return;
            flip(s, e); flip(s, e - k); flip(e - k + 1, e);
        }
    }

    void getidx(int k) {
        splay(ptr[k]);
        cout << root->l->sz << "\n";
    }

    void kth(int k) { //1-based
        auto now = root;
        push(now);
        push_sum(now);
        while (1) {
            while (now->l && now->l->sz > k) {
                now = now->l; push(now); push_sum(now);
            }
            if (now->l) k -= now->l->sz;
            if (!k) break; k--;
            now = now->r;
            push(now);
            push_sum(now);
        }
        splay(now);
    }

    void insert(ll key) {
        Node* p = root;
        Node** pp;
        if (!p) {
            Node* x = new Node;
            root = x;
            x->l = x->r = x->p = NULL;
            x->v = key; return;
        }
        while (1) {
            // If you dont want to make duplicated value, activate next code.
            //if (key == p->v) return;
            if (key < p->v) {
                if (!p->l) { pp = &p->l; break; }
                p = p->l;
            }
            else {
                if (!p->r) { pp = &p->r; break; }
                p = p->r;
            }
        }
        Node* x = new Node;
        *pp = x;
        x->l = x->r = NULL;
        x->p = p, x->v = key;
        splay(x);
    }

    bool find(ll key) {
        Node* p = root;
        if (!p) return false;
        while (p) {
            if (key == p->v) break;
            if (key < p->v) {
                if (!p->l) break;
                p = p->l;
            }
            else {
                if (!p->r) break;
                p = p->r;
            }
        }
        splay(p);
        return key == p->v;
    }

    void del(ll key) {
        if (!find(key)) return;

        Node* p = root;
        if (p->l && p->r) {
            root = p->l; root->p = NULL;
            Node* x = root;
            while (x->r) x = x->r;
            x->r = p->r; p->r->p = x;
            p->l = p->r = NULL;
            delete p; return;
        }
        if (p->l) {
            root = p->l; root->p = NULL;
            p->l = p->r = NULL;
            delete p; return;
        }
        if (p->r) {
            root = p->r; root->p = NULL;
            p->l = p->r = NULL;
            delete p; return;
        }
        p->l = p->r = NULL;
        delete p; root = NULL;
    }

    void print(Node* x) {
        push(x);
        if (x->l) print(x->l);
        if (!x->dummy) cout << x->v << " ";
        if (x->r) print(x->r);
    }
}tree;

int n, m, q;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0);
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> arr[i];
    tree.init(n);

    // tree.init : initiate tree to arr.
    // tree.insert(ll v) : insert value v to tree.
    // tree.del(ll v) : delete value v from tree.
    // tree.kth(int k) : make kth value node to root.
    // tree.gather(int s, int e) : return s ~ e subtree.
    // tree.flip(int s, int e) : make t[s] t[s + 1] ... t[e] to t[e] t[e - 1] ... t[s].
    // lazy propagation ; add v to [a, b]  
    //      tree.gather(a,b);
    //      Node *x = tree.root->r->l;
    //      x->sum += x->sz * c; x->lazy += c;
    //      tree.update(tree.root);
    // tree.shift(int s, int e, int k) : shift [s, e] with k.
    // tree.getidx(int k) : print kth element of array. (maybe tree)
    // tree.print(Node *x) : print subtree of x.
    

    cout << "program ends\n";
}
