//my solution
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if(!head &&!head->next)
            return head;
        int temp = head->val;
        ListNode* prev = head;
        ListNode* cur = head->next;
        
        while(cur!=nullptr){
            if(cur->val == temp){
                prev->next = cur->next;
                delete cur;
            }else{
                temp = cur->val;
                prev = cur;
            }
            cur = cur->next;
        }
        return head;
    }
};

//same solution but simpler
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        if (!head || !head->next) return head;
        
        ListNode *start = head;
        while (start && start->next) {
            if (start->val == start->next->val) {
                ListNode *tmp = start->next;
                start->next = start->next->next;
                delete tmp;
            } else start = start->next;
        }
        return head;
    }
};