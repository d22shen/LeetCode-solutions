//I didn't figure out a solution
//the following solutions are found online

//recursion without inorder traversal
// Recursion without inorder traversal
class Solution {
public:
    bool isValidBST(TreeNode *root) {
        return isValidBST(root, LONG_MIN, LONG_MAX);
    }
    bool isValidBST(TreeNode *root, long mn, long mx) {
        if (!root) return true;
        if (root->val <= mn || root->val >= mx) return false;
        return isValidBST(root->left, mn, root->val) && isValidBST(root->right, root->val, mx);
    }
};

//recursion with inorder traversal
//array to store the results
class Solution {
public:
    bool isValidBST(TreeNode *root) {
        if (!root) return true;
        vector<int> vals;
        inorder(root, vals);
        for (int i = 0; i < vals.size() - 1; ++i) {
            if (vals[i] >= vals[i + 1]) return false;
        }
        return true;
    }
    void inorder(TreeNode *root, vector<int> &vals) {
        if (!root) return;
        inorder(root->left, vals);
        vals.push_back(root->val);
        inorder(root->right, vals);
    }
};

//Still recursion with inorder traversal
//an int value to store the result and compare each time
//the same to the above solution
class Solution {
public:
    TreeNode *pre;
    bool isValidBST(TreeNode *root) {
        int res = 1;
        pre = NULL;
        inorder(root, res);
        if (res == 1) return true;
        else false;
    }
    void inorder(TreeNode *root, int &res) {
        if (!root) return;
        inorder(root->left, res);
        if (!pre) pre = root;
        else {
            if (root->val <= pre->val) res = 0;
            pre = root;
        }
        inorder(root->right, res);
    }
};

// Non-recursion with stack
class Solution {
public:
    bool isValidBST(TreeNode *root) {
        if (!root) return true;
        stack<TreeNode*> s;
        TreeNode *p = root;
        TreeNode *pre = NULL;
        while (p || !s.empty()) {
            while (p) {
                s.push(p);
                p = p->left;
            }
            p = s.top();
            s.pop();
            if (!pre) pre = p;
            else {
                if (p->val <= pre->val) return false;
            }
            pre = p;
            p = p->right;
        }
        return true;
    }
};