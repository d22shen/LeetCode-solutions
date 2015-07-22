//My solution with recursion 0ms
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(root == nullptr)
            return nullptr;
        if(root->left == nullptr&&root->right == nullptr)
            return root;
        
        if(root->left!=nullptr&&root->right!=nullptr){
            TreeNode *temp = invertTree(root->left);
            root->left = invertTree(root->right);
            root->right = temp;
            return root;
        }
        
        if(root->left!=nullptr&&root->right==nullptr){
            root->right = invertTree(root->left);
            root->left = nullptr;
            return root;
        }
        
        if(root->right!=nullptr&&root->left==nullptr){
            root->left = invertTree(root->right);
            root->right = nullptr;
            return root;
        }
    }
};

//same solution but much simpler than mine!!!!shittttt!
// Recursion
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return NULL;
        TreeNode *tmp = root->left;
        root->left = invertTree(root->right);
        root->right = invertTree(tmp);
        return root;
    }
};

//non-recursion
// Non-Recursion
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return NULL;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *node = q.front(); q.pop();
            TreeNode *tmp = node->left;
            node->left = node->right;
            node->right = tmp;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return root;
    }
};