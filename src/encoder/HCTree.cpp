#include "HCTree.hpp"

/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() {
    if (root) root->rDelete();
}

void HCNode::rDelete() {
    if (this->c0) {
        c0->rDelete();
    }
    if (this->c1) {
        c1->rDelete();
    }
    delete this;
}

HCNode* HCTree::returnRoot() { return root; }

/**
 * TODO: Build the HCTree from the given frequency vector. You can assume the
 * vector must have size 256 and each value at index i represents the frequency
 * of char with ASCII value i. Only non-zero frequency symbols should be used to
 * build the tree. The leaves vector must be updated so that it can be used in
 * encode() to improve performance.
 *
 * When building the HCTree, you should use the following tie-breaking rules to
 * match the output from reference solution in checkpoint:
 *
 *    1. HCNode with lower count should have higher priority. If count is the
 * same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher priority
 * node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (int index = 0; index < 256; index++) {
        if (freqs[index] > 0) {
            HCNode* newNode = new HCNode(freqs[index], index);
            leaves[index] = newNode;
            pq.push(newNode);
        }
    }

    while (pq.size() > 1) {
        HCNode* c0 = pq.top();
        pq.pop();
        HCNode* c1 = pq.top();
        pq.pop();

        HCNode* parent =
            new HCNode(c0->count + c1->count, c0->symbol, c0, c1, NULL);
        c0->p = parent;
        c1->p = parent;

        pq.push(parent);
    }

    root = pq.top();
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream.
 * You must not perform a comprehensive search to find the encoding bits of
 * the given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been
 * called beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    HCNode* node = leaves[symbol];
    string code;
    if (node == NULL) return;

    if (node == root) {
        out << "0";
        return;
    }

    while (node->p) {
        if (node == node->p->c0) {
            code += "0";
        } else if (node == node->p->c1) {
            // cout << "comes here";
            code += "1";
        }
        node = node->p;
    }

    for (int index = code.length() - 1; index >= 0; index--) {
        out << code.at(index);
    }
}

/**
 * TODO: Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to
 * work, build() must have been called beforehand to create the HCTree.
 */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or
 * '1') from istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    unsigned char c = in.get();
    HCNode* curr = root;

    while (c != 255 && curr) {
        if (curr->c0 == NULL && curr->c1 == NULL) {
            if (curr != root) in.unget();
            return curr->symbol;
        }
        // cout << "Char is" << c << endl;
        if (c == '0') {
            curr = curr->c0;
        } else if (c == '1') {
            curr = curr->c1;
        }
        c = in.get();
    }

    return curr->symbol;
}
