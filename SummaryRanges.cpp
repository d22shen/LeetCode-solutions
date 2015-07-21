//user a string vector to store the ranges
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> ranges;
        if(nums.empty())
            return ranges;
        int i=0;
        for(i=0;i<nums.size()-1;i++){
            int j = i;
            while((nums[j]+1)==nums[j+1]&&j<nums.size()-1){
                j++;
            }
            if(j == i){
                ranges.push_back(to_string(nums[j]));
            }else{
                ranges.push_back(to_string(nums[i]) + "->" + to_string(nums[j]));
                i = j;
            }
        }
        if(i==nums.size()-1)
            ranges.push_back(to_string(nums[i]));
        return ranges;
    }
    
};

//online solution
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> range(0);
        if( nums.size() == 0 ){
            return range;
        }
        vector<int>::iterator iter = nums.end() - 1;
        if( *iter == -1 ){
            nums.push_back(2);
        }else{
            nums.push_back(0);
        }
        iter = nums.begin();
        for (vector<int>::iterator iter1 = nums.begin(); iter1 != nums.end() - 1; ++iter1){
            if( *iter1 + 1 != *(iter1 + 1) ){
                string rangeString = (iter == iter1)?(to_string( *iter)):(to_string(*iter)+"->"+to_string(*iter1));
                range.push_back(rangeString);
                iter = iter1 + 1;
            }
        }
        return range;
    }
};

//my initial solution
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> ranges;
        if(nums.empty())
            return ranges;
        if(nums.size() == 1){
            ranges.push_back(to_string(nums.front()));
            return ranges;
        }
        int i=0;
        for(i=0;i<nums.size()-1;i++){
            int j = i;
            while((nums[j]+1)==nums[j+1]&&j<nums.size()-1){
                j++;
            }
            if(j == i){
                ranges.push_back(to_string(nums[j]));
            }else{
                ranges.push_back(to_string(nums[i]) + "->" + to_string(nums[j]));
                i = j;
            }
        }
        if(i==nums.size()-1)
            ranges.push_back(to_string(nums[i]));
        return ranges;
    }
};