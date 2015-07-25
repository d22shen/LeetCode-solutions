//my solution
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        if(!head) return nullptr;
        ListNode *prev = nullptr;
        ListNode *cur = head;
        while(cur!=nullptr){
            ListNode *temp = cur->next;
            if(cur->val == val){
                if(prev){
                    delete cur;
                    prev->next = temp;
                    cur = temp;
                }else{
                    head = temp;
                    delete cur;
                    cur = temp;
                }
                
            }else{
                prev = cur;
                cur = temp;
            }
        }
        return head;
    }
};

//similar solution but a dummy node
//haha, a dummy node to serve as the initial prev node
//hence, the guy does not need to check the nullptr prev condition
//smart
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode *dummy = new ListNode(-1);
        dummy->next = head;
        ListNode *pre = dummy, *cur = head;
        while (cur) {
            if (cur->val == val) {
                ListNode *tmp = cur;
                pre->next = cur->next;
                cur = cur->next;
                delete tmp;
            } else {
                cur = cur->next;
                pre = pre->next;
            }
        }
        return dummy->next;
    }
};