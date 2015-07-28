//my solution
//same thought but I didn't delete the node
class Solution {
public:
    void deleteNode(ListNode* node) {
        node->val = node->next->val;
        node->next = node->next->next;
    }
};


//grandyang's solution
class Solution {
public:
    void deleteNode(ListNode* node) {
        node->val = node->next->val;
        ListNode *tmp = node->next;
        node->next = tmp->next;
        delete tmp;
    }
};
