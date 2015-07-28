//my first try
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(!l1) return l2;
        if(!l2) return l1;
        ListNode *head = nullptr, *temp = nullptr;
        if(l2->val<=l1->val) {
            head = l2;
            temp = l2;
            l2 = l2->next;
            while(l1!=nullptr&&l2!=nullptr){
                if(l1->val<=l2->val){
                    temp->next = l1;
                    l1 = l1->next;
                }else{
                    temp->next = l2;
                    l2 = l2->next;
                }
                temp = temp->next;
            }
            if(!l1){
                temp->next = l2;
            }else{
                temp->next = l1;
            }
            
        }else {
            head = l1;
            temp = l1;
            l1 = l1->next;
            while(l1!=nullptr&&l2!=nullptr){
                if(l1->val<=l2->val){
                    temp->next = l1;
                    l1 = l1->next;
                }else{
                    temp->next = l2;
                    l2 = l2->next;
                }
                temp = temp->next;
            }
            if(!l1){
                temp->next = l2;
            }else{
                temp->next = l1;
            }
        }
        return head;
    }
};

//simplified solution but same thought
class Solution {
public:
    ListNode* getSmaller(ListNode* l1, ListNode* l2){
        if(l1->val<=l2->val){
            return l1;
        }else{
            return l2;
        }
    }
    ListNode* getBigger(ListNode* l1, ListNode* l2){
        if(l1->val>l2->val){
            return l1;
        }else{
            return l2;
        }
    }
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(!l1) return l2;
        if(!l2) return l1;
        ListNode *small = getSmaller(l1,l2), *big = getBigger(l1,l2);
        ListNode *head = small, *temp = small;
        small = small->next;
        while(small!=nullptr&&big!=nullptr){
            if(small->val<=big->val){
                temp->next = small;
                small = small->next;
            }else{
                temp->next = big;
                big = big->next;
            }
            temp = temp->next;
        }
        if(!small){
            temp->next = big;
        }else{
            temp->next = small;
        }
        
        return head;
    }
};

//shit, learn to create a dummy node, u dumbass!!
class Solution {
public:
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
        ListNode *res = new ListNode(0);
        ListNode *cur = res;
        while(l1 && l2) {
            if (l1->val < l2->val) {
                cur->next = l1;
                l1 = l1->next;
            } else {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next;
        }
        if (l1) cur->next = l1;
        if (l2) cur->next = l2;
        return res->next;
    }
};














