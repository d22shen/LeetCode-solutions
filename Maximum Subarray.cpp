//dynamic programming
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int res = nums[0], tmp = nums[0];
        for(int i=1;i<nums.size();i++){
            tmp = max(tmp+nums[i],nums[i]);
            res = max(res,tmp);
        }
        return res;
    }
};


//divide and conquer
class Solution {
public:
    //divide and conquer
    int maxSubArray(vector<int>& nums) {
        // IMPORTANT: Please reset any member data you declared, as
        // the same Solution instance will be reused for each test case.
        if(!nums.size()) return 0;
        return maxSubArrayHelperFunction(nums, 0, nums.size()-1);
    }
    
    int maxSubArrayHelperFunction(vector<int>& nums, int left, int right) {
        if(right == left) return nums[left];
        int middle = (left+right)/2;
        int leftans = maxSubArrayHelperFunction(nums, left, middle);
        int rightans = maxSubArrayHelperFunction(nums, middle+1, right);
        int leftmax = nums[middle];
        int rightmax = nums[middle+1];
        int temp = 0;
        for(int i=middle;i>=left;i--) {
            temp += nums[i];
            if(temp > leftmax) leftmax = temp;
        }
        temp = 0;
        for(int i=middle+1;i<=right;i++) {
            temp += nums[i];
            if(temp > rightmax) rightmax = temp;
        }
        return max(max(leftans, rightans),leftmax+rightmax);
    }
    
    
};
