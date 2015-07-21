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

//the following code is way faster than mine
//only bit manipulation, no math calculation
//check the last bit, shift the res to left and add the the last 1 or zero
//shift the n to the right
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t res = 0;
        for (int i = 0; i < 32; ++i) {
            if (n & 1 == 1) {
                res = (res << 1) + 1;
            } else {
                res = res << 1;
            }
            n = n >> 1;
        }
        return res;
    }
};
//the same solution but less code
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t res = 0;
        for (int i = 0; i < 32; ++i) {
            res |= ((n >> i) & 1) << (31 - i);
        }
        return res;
    }
};