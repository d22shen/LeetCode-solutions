
//the difficult is to check the overflow of the integer
class Solution {
public:
    int reverse(int x) {
        int  res = 0;
        while (x!=0){
            if (res > INT_MAX/10 || res < INT_MIN/10){
                return 0;
            }
            res = res*10+ x%10;
            x= x/10;
        }
        return res;
    }
};

//another online solution
class Solution{
public:
    int reverse(int x){
        int newN = 0;
        int sign = 1;
        if(x<0){
            sign = -1;
            x = abs(x);
        }
        while(x/10!=0)
        {
            newN = newN*10 + x%10;
            x = x/10;
        }
        if(newN*10/10!=newN)
            return 0;
        newN = newN*10;
        if((newN+x%10-x%10)!=newN)
            return 0;
        newN = newN+x;
        return newN*sign;
    }
};