//sup online solution
class Solution {
public:
    bool isPowerOfTwo(int n){
        if(n<0)
            return false;
        int count = 0;
        for(count=0;n;count++){
            n &= (n-1);
        }
        if(count==1){
            return true;
        }
        return false;
    }
    
};

//easy to understand
//count the 1 bits in the number
//shift the string to right each time, and check the last bit to see whether it is 1 bit
//if there is only one 1-bit, then it is the power of two
class Solution {
public:
    bool isPowerOfTwo(int n){
        int count = 0;
        while(n>0){
            if((n&1)==1){
                count++;
            }
            n>>=1;
        }
        if(count==1){
            return true;
        }
        return false;
    }
};