/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

//recursion of mine
class Solution {
public:
    int minDepth(TreeNode* root) {
        if(root == NULL)
            return 0;
        if(root->left == NULL)
            return minDepth(root->right)+1;
        if(root->right == NULL)
            return minDepth(root->left)+1;
        
        //>= is wrong
        //> is right
        //<= is right
        //wierd
        if(minDepth(root->left)>minDepth(root->right)){
            return minDepth(root->right) + 1;
        }else{
            return minDepth(root->left) + 1;
        }
    }
};