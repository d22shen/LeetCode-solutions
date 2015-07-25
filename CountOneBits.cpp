//aha
//what I learned
//simplest solution but not the best
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int c = 0;
        while(n!=0){
            if(n&1 == 1)
                c++;
            n>>=1;
        }
        return c;
    }
};