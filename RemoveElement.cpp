//my solution
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        if(nums.empty()) return 0;
        for(int i=0;i<nums.size();i++){
            if(nums[i] == val){
                nums.erase(nums.begin()+i);
                i--;
            }
        }
        return nums.size();
    }
};

//Similar solution
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int res = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != val) nums[res++] = nums[i];
        }
        return res;
    }
};
