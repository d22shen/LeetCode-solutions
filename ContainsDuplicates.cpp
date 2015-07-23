//my solution
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        int s = nums.size();
        unordered_map<int, int> numsH;
        for(int i=0;i<s;i++){
            numsH[nums[i]] = i;
        }
        if(numsH.size() == s)
            return false;
        return true;
    }
};

//basically the same
//but learn the programming style!!
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); ++i) {
            if (m.find(nums[i]) != m.end()) return true;
            ++m[nums[i]];
        }
        return false;
    }
};

//sort first
//then check
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == nums[i - 1]) return true;
        }
        return false;
    }
};