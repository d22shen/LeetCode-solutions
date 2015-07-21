//my solution
//shift the n to 1 bit to the right each time until it is zero
//use math to calculate the actual value it will add to the final result
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t temp = 0, i = 0;
        while(n){
            temp += pow(2,31-i)*(n&1);
            n>>=1;
            i++;
        }
        return temp;
    }
};