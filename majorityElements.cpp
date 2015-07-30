//first try with hash map
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int,int> mymap;
        for(int i=0;i<nums.size();i++){
            mymap[nums[i]] = 0;
        }
        for(int i=0;i<nums.size();i++){
            mymap[nums[i]]++;
        }
        for(int i=0;i<nums.size();i++){
            if(mymap[nums[i]]>nums.size()/2) return nums[i];
        }
    }
};
//wow, the voting method
class Solution {
public:
    int majorityElement(vector<int> &num) {
        int Idx = 0, count = 1;
        for (int i = 1; i < num.size(); ++i) {
            num[Idx] == num[i] ? ++count : --count;
            if (count == 0) {
                Idx = i;
                count = 1;
            }
        }
        return num[Idx];
    }
};
