//p and q are two nodes in the BST
//this is a binary search tree
//left<root<right
//just check the max of p&q and min of p&q with root
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(!root) return nullptr;
        if(max(p->val,q->val)<root->val) return lowestCommonAncestor(root->left, p, q);
        if(min(p->val, q->val)>root->val) return lowestCommonAncestor(root->right, p, q);
        return root;
    }
};

//non-recursion
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (true) {
            if (root->val > max(p->val, q->val)) root = root->left;
            else if (root->val < min(p->val, q->val)) root = root->right;
            else break;
        }
        return root;
    }
};