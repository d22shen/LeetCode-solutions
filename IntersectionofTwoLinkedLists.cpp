/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
//get the interaction of two linked lists
//if one same node is in both lists, then there is interaction
//time O(n) and space O(1)
class Solution {
public:
    int getLength(ListNode *head){
        int c = 0;
        while(head){
            c++;
            head = head->next;
        }
        return c;
    }
    
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if(headA==nullptr||headB==nullptr)
            return nullptr;
        ListNode *p1,*p2;
        p1=headA,p2=headB;
        int lenA=getLength(headA),lenB=getLength(headB);
        while(lenA<lenB){
            p2=p2->next;
            lenA++;
        }
        while(lenB<lenA){
            p1=p1->next;
            lenB++;
        }
        while(p1&&p2){
            if(p1==p2)
                return p1;
            p1=p1->next;
            p2=p2->next;
        }
        return nullptr;
    }
};

//my solution