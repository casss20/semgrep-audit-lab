#include <iostream>

class BTreeNode {
public:
    typedef BTreeNode* BTreeNodePtr;
    int *keys;
    int t;
    BTreeNodePtr *children;
    int n;
    bool leaf;

    BTreeNode(int _t, bool _leaf) {
        t = _t;
        leaf = _leaf;
        keys = new int[2 * t - 1];
        children = new BTreeNodePtr[2 * t];
        n = 0;
        for (int i = 0; i < 2 * t; i++) children[i] = nullptr; // ✅ Init to nullptr
    }

    void insertNonFull(int k);
    void splitChild(int i, BTreeNode *y);
    void traverse();
};

class BTree {
    BTreeNode *root;
    int t;

public:
    BTree(int _t) {
        root = nullptr;
        t = _t;
    }

    void traverse() {
        // ✅ Added nullptr check for root
        if (root != nullptr) root->traverse();
    }

    void insert(int k);
};

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (!leaf && children[i] != nullptr)  // ✅ Null check
            children[i]->traverse();
        std::cout << " " << keys[i];
    }

    if (!leaf && children[i] != nullptr)  // ✅ Null check
        children[i]->traverse();
}

void BTree::insert(int k) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);
            s->children[0] = root;

            // ✅ Check for null before calling
            if (s != nullptr && root != nullptr)
                s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k) i++;

            if (s->children[i] != nullptr)  // ✅ Null check
                s->children[i]->insertNonFull(k);

            root = s;
        } else {
            if (root != nullptr)  // ✅ Just in case
                root->insertNonFull(k);
        }
    }
}

void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n = n + 1;
    } else {
        while (i >= 0 && keys[i] > k)
            i--;

        // ✅ Bounds + null check before accessing child
        if (i + 1 < 2 * t && children[i + 1] != nullptr) {
            if (children[i + 1]->n == 2 * t - 1) {
                splitChild(i + 1, children[i + 1]);
                if (keys[i + 1] < k)
                    i++;
            }

            if (children[i + 1] != nullptr)
                children[i + 1]->insertNonFull(k);
        }
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
    if (y == nullptr) return;  // ✅ Defensive programming

    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    y->n = t - 1;

    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];

    children[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];

    n = n + 1;
}
/*

ID	Type	Description	Risk	Fix Applied
A1	Null Dereference	children[i + 1] accessed without bounds/null check	High	Added if (i + 1 < 2 * t && children[i + 1] != nullptr)
A2	Unsafe Root Use	root->traverse() and root->insertNonFull(k) used without check	Medium	Wrapped in if (root != nullptr)
A3	Unsafe SplitChild	splitChild passed root and dereferenced y->keys[t - 1] without checking	Medium	Added if (y == nullptr) return; and guards
A4	False Positives	Some guarded expressions still flagged by Semgrep	Low	Verified safe by manual review

*/