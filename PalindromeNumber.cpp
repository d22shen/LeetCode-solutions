//my solution
class Solution {
public:
    bool isPalindrome(int x) {
        if(x<0) return false;
        int v = x;
        int temp = 0;
        while(v){
            temp = temp*10+v%10;
            v/=10;
        }
        if(temp == x) return true;
        return false;
    }
};

//grandyang solution 132ms
//get the head and last at meanwhile
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0) return false;
        int div = 1;
        while (x / div >= 10) div *= 10;
        while (x > 0) {
            int left = x / div;
            int right = x % 10;
            if (left != right) return false;
            x = (x % div) / 10;
            div /= 100;
        }
        return true;
    }
};