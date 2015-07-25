//my solution
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if(root == nullptr)
            return false;
        if(root->left == nullptr&&root->right == nullptr&&root->val == sum)
            return true;
        if(root->left&&hasPathSum(root->left, sum - root->val))
            return true;
        if(root->right&&hasPathSum(root->right, sum - root->val))
            return true;
        return false;
    }
};

//online solution
class Solution {
public:
    bool hasPathSum(TreeNode *root, int sum) {
        if (root==NULL) {return false;}
        if ( (root->left==NULL) && (root->right==NULL) && (sum-root->val==0) ) {return true;}
        return (hasPathSum(root->left, sum-root->val) || hasPathSum(root->right, sum-root->val));
    }
};

