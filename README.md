# FAZBD UDF&UDAF For Impala


## Setup

All commands below should be run under the project root (fazudf/)

Make sure you have aws cli preinstalled
aws default region is us-west-1, default output is None

```bash
# 1. Setup env
aws configure
eval $(aws ecr get-login --no-include-email | sed 's|https://||')

# 2. Create base docker image
./base_docker/build_base_docker.sh

# 3. Build .so file with mvn
mvn package

# 4. Start all (Impala-Kudu docker and udf)
docker-compose -f docker-compose-udf.yml up

# 5. Stop all
docker-compose -f docker-compose-udf.yml down

# 6. User Impala-Kudu
docker exec -it fazudf_impala-udf_1 /bin/bash

# 7. Start impala-shell
impala-shell

# 8. Use the db_public_kudu database
use db_public_kudu;

# 9. Show all Impala UDFs
show functions;
```

# UDF&UDAF Spec and Test Cases

## limitations

FAZ's UDFs are for Postgres Database. Postgres supports abundant built-in data types and customized data type, but Impala does not support them. Hence, we have to use string to support these complex data types. For example, we have to use string to represent int4range, array, fct_uuid, inet and use int or double to represent smallint or numeric. For now, Impala UDF only supports IntVal, BigIntVal, SmallIntVal, TinyIntVal, StringVal, BooleanVal, FloatVal, DoubleVal, TimestampVal. The complex types in PostgresSQL are represented by string as follows:

- int\[\] (integer array) => "{3,2,1,0}"(int values surrounded by braces)
+ int4range (range of integer) => "[3,7)" (two int values surrounded by square brackets or parentheses; '[' means lower bound inclusive; ']' means upper bound inclusive; '(' means lower bound exclusive; ')' means upper bound exlcusive)
- int4range\[\] (array of int4range) => "{"[3,7)","[8,11)"}" (int4range values surrounded by braces; Remember the quotation marks that must not be neglected!!!)
+ numeric (big decimal number) => "9999999999999999999999999999" (Shoud support decimal(28,0) but can only support with "long long" in C++)
- inet (address/y, address is ipv4 or ipv6 and y is binary bits number of subnet mask by default 32 for ipv4 and 128 for ipv6) => "192.168.31.0/24"

## UDFs

```bash
1. threatweight_sum(string arg1, string arg2) returns int
    arg1: threatweights array (int array)
    arg2: threatcounts array (int array)
    return: sum (threatweight*threatcount)
    limitations:
        1. arg1 and arg2 should be arrays with same number of elements. Otherwise, zero would be returned.
    test case 1 arrays with same number of elements:
        query: select threatweight_sum('{1,1,1,1,1}', '{1,2,3,4,5}');
        result: 15 (1*1+1*2+1*3+1*4+1*5)
    test case 2 arrays with different number of elements:
        query: select threatweight_sum('{1,1,1,1,1}', '{1,2,3,4}');
        result: 0

2. threatweight_level_sum(int arg1, string arg2, string arg3, string arg4) returns int
    arg1: threatlevel (4(cri), 3(hi), 2(med), 1(low))
    arg2: threatlevels array (int array)
    arg3: threatcounts array (int array)
    arg4: threatweights array (int array)
    return: sum of (threatweight*threatcount) for specified threatlevel
    limittations:
        1. arg1, arg2 and arg3 should be arrays with same number of elements. Otherwise, zero would be returned.
        2. threatlevel range from 1 to 4 inclusive
    test case 1 arrays with same number of elements:
        query: select threatweight_level_sum(1, '{1,2,3,4,1}', '{1,2,3,4,5}', '{1,1,1,1,1}');
        result: 6 (1*1+1*5=6 when input threatlevel is 1)
    test case 2 arrays with different number of elements:
        query: select threatweight_level_sum(1, '{1,2,3,4,1}', '{1,2,3,4,5}', '{1,1,1}');
        result: 0
    test case 3 arrays with threatlevel out of range:
        query: select threatweight_level_sum(6, '{6,6,6,6,6}', '{1,2,3,4,5}', '{1,1,1,1,1}');
        result: 0

3. threatlevel_max(string arg1) returns int
    arg1: threatlevels array (int array)
    return: max threatlevel from the input threatlevels array
    test case 1 threatlevels within range:
        query: select threatlevel_max('{1,2,3,4,1}');
        result: 4
    test case 2 threatlevels out of range:
        query: select threatlevel_max('{1,2,3,4,1111}');
        result: 1111 (follow faz design)

4. root_domain(string arg1) returns string
    arg1: hostname string
    return: domain name from given hostname
    Three main cases are considered here:
    1. for most cases like hostname = 'www.fortinet.com', last two sections are kept as domain name
    2. special cases like hostname = 'sina.com.cn', the last two sections will be checked and if they both are in top domain list then kept one more section in the domain name
    3. if hostname is actually an ip4 or ip6 address with or without port number, the whole address is returned as domain name
    test case 1 most cases like 'www.fortinet.com':
        query: select root_domain('www.fortinet.com');
        result: 'fortinet.com' (last two sections are kept as domain name)
    test case 2 special cases like 'www.sina.com.cn':
        query: select root_domain('www.sina.com.cn');
        result: 'sina.com.cn' (last two sections will be checked if they both are in top domain list then kept one more section in the domain name)
    test case 3 ip address:
        query: select root_domain('2002:cc:ef::16');
        result: '2002:cc:ef::16' (the whole address is returned as domain name)
    test case 4 special case ip address with square brackets and port number:
        query: select root_domain('https://[2002:cc:ef::16]:443/');
        result: '2002:cc:ef::16' (only the ip section is kept)
    test case 5 special case like 'www.am.com':
        query: select root_domain('www.am.com');
        result: 'am.com' (both "am" and "com" are in top domain map, but "am" here is not a top domain should be kept)
    test case 6 special case like 'www.abc.am.com':
        query: select root_domain('www.abc.am.com');
        result: 'abc.am.com' (both "am" and "com" are in top domain map, but "am" here is a top domain should be kept as well)
    TODO issue: test case 2 succeeds by chance because the helper function 'lookup_domain_info' is not implemented yet.
        'lookup_domain_info' is to check whether some domain is in top domain list. FAZBD should also have this helper function.

5. nullifna(string arg1) returns string
    arg1: any string
    return: string same as input or NULL if input is 'N/A' or 'n/a'
    test case 1 string with is not 'N/A' or 'n/a'
        query: select nullifna('aaaaa');
        result: 'aaaaa'
    test case 2 'N/A':
        query: select nullifna('N/A');
        result: NULL
    test case 3 'n/a':
        query: select nullifna('n/a');
        result: NULL

6. app_group_name(string arg1) returns string
    arg1: application group name that should be two string tokens connected by underscore
    return: first string token before first underscore or same as input if no underscore
    test case 1 input with underscore:
        query: select app_group_name('app_group_name');
        result: 'app'
    test case 2 input without underscore:
        query: select app_group_name('appgroupname');
        result: 'appgroupname'

7. is_ip(string arg1) returns boolean
    arg1: ip string
    return: true if input is valid ip or false if input is invalid ip
    test case 1 valid ipv4 address:
        query: select is_ip('192.168.6.108');
        result: true
    test case 2 invalid ipv4 address:
        query: select is_ip('192.168.6.266');
        result: false
    test case 3 invalid ipv4 address:
        query: select is_ip('192.168.5');
        result: false
    test case 4 valid ipv6 address:
        query: select is_ip('2001:3CA1:010F:001A:121B:0000:0000:0010');
        result: true
    test case 5 invalid ipv6 address:
        query: select is_ip('2001:3CA1:010F:001A:121B:0000:0000');
        result: false

8. from_itime(bigint arg1) returns string
    arg1: itime bigint (UTC generated by FAZ when it receives a log that is FAZ local time)
    return: date time string in the FAZBD local time zone
    limitations:
        1. This does not make sense for FAZBD that is in different time zone from the FAZ
        2. Better use from_unixtime(bigint) instead to avoid the time zone issue
    test case 1 input of zero:
        query: select from_itime(0);
        result: '1970-01-01 08:00:00' (FAZ and FAZBD local time zone will affect this value)

9. from_dtime(bigint arg1) returns string
    arg1: dtime bigint (UTC calculated by FAZ using 'date' and 'time' fields received from the Fortigate)
    return: date time string in GMT timezone
    limitations:
        1. It is not GMT time in fact, so there are timezone differences between dtime and real unix timestamp
        2. FAZ uses this method to avoid time zone issue for date time string value
    test case 1 input of zero:
        query: select from_dtime(0);
        result: '1970-01-01 00:00:00' (Epoch Start Date Time)

10. email_user(string arg1) returns string
    arg1: email address string (valid email address should be follow 'emailuser@emaildomain' format such as 'd22shen@fortinet.com')
    return: email user section before '@'
    test case 1 valid email address with '@':
        query: select email_user('dsshen@fortinet.com');
        result: 'dsshen' (token before '@')
    test case 2 invalid email address without '@':
        query: select email_user('dsshenfortinet.com');
        result: 'dsshenfortinet.com'

11. email_domain(string arg1) returns string
    arg1: email address string
    return: email domain section after '@'
    test case 1 valid email address with '@':
        query: select email_domain('dsshen@fortinet.com');
        result: 'fortinet.com' (token after '@')
    test case 2 invalid email address without '@':
        query: select email_domain('dsshenfortinet.com');
        result: empty output

12. date_time(bigint arg1) returns string
    arg1: UTC bigint
    return: human readable date time string in local timezone in 'MM/DD hh24:mi' format
    test case 1 input of zero:
        query: select date_time(0);
        result: '01/01 08:00' (local timezone may affect this value)

13. tmtodate(bigint arg1) returns string
    arg1: UTC bigint
    return: human readable date string in local timezone in 'YYYY-MM-DD' format
    test case 1 input of zero:
        query: select tmtodate(0);
        result: '1970-01-01' (local timezone may affect this value)

14. tmtohour(bigint arg1) returns string
    arg1: UTC bigint
    return: human readable date time string in local timezone in 'YYYY-MM-DD HH24:00' format
    test case 1 input of zero:
        query: select tmtohour(0);
        result: '1970-01-01 08:00' (local timezone may affect this value)

15. logid_to_int(string arg1) returns int
    arg1: logid string
    return: cast the tailing 6 digits to int
    test case 1 valid logid:
        query: select logid_to_int('0001000014');
        result: 14
    test case 2 invalid logid:
        query: select logid_to_int('0014');
        result: 14 (follow FAZ design)

16. bigint_array_sum(string arg1) returns bigint
    arg1: bigint array
    return: sum of the bigint array elements
    test case 1 valid bigint array:
        query: select bigint_array_sum('{1000000000000,200000000000,300000000000,4000000000000,5000000000000}');
        result: 10500000000000
    test case 2 bigint max overflow:
        query: select bigint_array_sum('{9223372036854775807,1}');
        result: "-9223372036854775808" (same with FAZ)
    test case 3 negative value:
        query: select bigint_array_sum('{-1000000000000,200000000000,300000000000,-4000000000000,5000000000000}');
        result: 500000000000
    test case 4 bigint min overflow:
        query: select bigint_array_sum('{-9223372036854775808,-1}');
        result: 9223372036854775807 (same with FAZ)

17. string_position(string arg1, string arg2) returns int
    arg1: string
    arg2: string
    return: first occurence of arg1 in arg2 (index starts from 1 while 0 means not found)
    test case 1 found:
        query: select string_position('add', 'addiii');
        result: 1
    test case 2 not found:
        query: select string_position('add', 'adiiii');
        result: 0

18. file_name_ext(string arg1) returns string
    arg1: file name with type suffix separated with '.'
    return: file type suffix
    test case 1 valid file name with single suffix:
        query: select file_name_ext('readme.md');
        result: 'md'
    test case 2 file name without suffix
        query: select file_name_ext('readme');
        result: NULL
    test case 3 file name with multiple suffix
        query: select file_name_ext('readme.txt.md');
        result: 'md' (return last suffix)

19. ipstr(string arg1) returns string
    arg1: inet string with or without binary bits number of subnet mask
    return: the ip section of the inet string input
    limitations:
        1. inet type validation check is not implemented.
    test case 1 valid ip input:
        query: select ipstr('192.168.6.108');
        result: '192.168.6.108'
    test case 2 invalid ip input:
        query: select ipstr('192.168.6.256');
        result: NULL (different from FAZ since postgres will check inet data type automatically)
    test case 3 valid inet input with mask:
        query: select ipstr('192.168.31/24');
        result: '192.168.31.0'
    TODO issue: inet data type should be checked considering the ip section and the subnet mask. ipv6 should be also considered.

20. ip_subnet(string arg1) returns string
    arg1: inet string with or without subnet mask bits number
    return: ip subnet string
    TODO issue: just find out this does not work for now. The algorithm is kind of complex.

21. vpn_trim(string arg1) returns string
    arg1: vpn string
    return: trim vpn string with regex '_[0-9]+$'
    1. When input argument is ended with 'underscore plus numbers' such as "_123", the ending underscore and numbers will be removed.
    2. Otherwise, return string will be the same as input.
    test case 1 vpn string match the regex:
        query: select vpn_trim('aaaa_11111');
        result: 'aaaa'
    test case 2 vpn string not match regex:
        query: select vpn_trim('aaaa11111');
        result: 'aaaa11111'

22. fctver_trim(string arg1) returns string
    arg1: forticlient version string
    return: trim fctver string with regex '^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}'
    1. Input should also follow the regex format 'XXX.XXX.XXXSSSSSS' (X is a digit from 0 to 9 while S can be any character).
    2. The tailing SSSS section after XXX.XXX.XXX will be removed.
    test case 1 valid input match the regex:
        query: select fctver_trim('111.111.11111111');
        result: '111.111.111'
    test case 2 invalid input that does not match the regex:
        query: select fctver_trim('11111.111.11111111');
        result: NULL

23. format_numeric_no_decimal(string arg1) returns string
    arg1: numeric string that is designed to be decimal(28,0) but overflow happens easily since C++ long long is used to represent the numeric value
    return: round the numeric number and return only the integer section
    test case 1 normal case:
        query: select format_numeric_no_decimal('55555.55');
        result: '55556'
    test case 2 negative input:
        query: select format_numeric_no_decimal('-55555.55');
        result: '-55556'
    test cases to be added including input overflow etc.
    TODO issue: numeric should be decimal(28,2) but for now our numeric can only support number less than 20 digits including signs

24. to_string(anyelement arg1) returns string
    arg1: input can be of int, bigint, smallint, string, boolean, float, double, timestamp data types
    return: cast input as string
    test case 1 input is int:
        query: select to_string(cast(111 as int));
        result: '111'
    test case 2 input is bigint:
        query: select to_string(cast(111 as bigint));
        result: '111'
    test case 3 input is smallint:
        query: select to_string(cast(111 as smallint));
        result: '111'
    test case 4 input is string:
        query: select to_string('I am a string!');
        result: 'I am a string!'
    test case 5 input is boolean:
        query: select to_string(cast(0 as boolean));
        result: 'false'
    test case 6 input is float:
        query: select to_string(cast(111.11 as float));
        result: '111.11'
    test case 7 input is double:
        query: select to_string(cast(111.11 as double));
        result: '111.11'
    test case 8 input is timestamp:
        query: select to_string(cast('1970-01-01 00:00:00.0000' as timestamp));
        result: '1970-01-01 00:00:00'

25. bandwidth_unit(string arg1) returns string
    arg1: input is numeric string indicating bandwidth in bytes
    return: transform input bytes to GB, MB and KB with two decimal places
    test case 1 input is GB level:
        query: select bandwidth_unit('1073741824');
        result: '1.00GB'
    test case 2 input is MB level:
        query: select bandwidth_unit('1048576');
        result: '1.00MB'
    test case 3 input is KB level:
        query: select bandwidth_unit('1024');
        result: '1.00KB'
    test case 4 input is less than 1KB:
        query: select bandwidth_unit('1023');
        result: '1023.00'

26. fv_timescale_func(bigint arg1, int arg2, int arg3) returns bigint
    arg1: bigint
    arg2: int
    arg3: int
    return: result of ((arg1 + arg3)/arg2 * arg2 - arg3)
    limitations:
        1. use case of this function is not quite clear
    test case 1:
        query: select fv_timescale_func(100, 2, 3);
        result: 99

27. virusid_to_str(bigint arg1, string arg2) returns string
    arg1: virusid bigint
    arg2: botnet prefix
    return: virusid string if arg2 is not botnet; otherwise, botnet:virusid
    test case 1 input with botnet prefix
        query: select virusid_to_str(1000000,'botnet');
        result: 'botnet:1000000'
    test case 2 input with 'other' prefix:
        query: select virusid_to_str(1000000,'haha');
        result: '1000000'
    test case 3 input with no prefix:
        query: select virusid_to_str(1000000,'');
        result: '1000000'

28. extract_epoch(string arg1) returns bigint
    arg1: time string or date time string (date will be ignored)
    return: epoch in seconds calculated from the time section of arg1
    test case 1 valid date time string:
        query: select extract_epoch('1970-01-01 00:00:00.0000');
        result: 0
    test case 2 valid time string:
        query: select extract_epoch('10:00:00.0000');
        result: 36000
    test case 3 no time string:
        query: select extract_epoch('1970-01-01');
        result: NULL

29. fctos_to_devtype(string arg1) returns string
    arg1: forticlient OS string (this is filled by some fortinet device and is hard coded)
    return: device type generated from arg1
    limitations:
        1. valid fct os is hard-coded including 'Windows', 'Mac', 'iPad OS', 'iPhone OS', 'iPod OS', 'Android Phone', and 'Android Tablet'
        2. These fct device type should be obtained from the faz logs to add more test cases
    test case 1 predefined fctos:
        query: select fctos_to_devtype('Microsoft Windows 7 Enterprise Edition, 32-bit Service Pack 1 (build 7601)');
        result: 'Windows PC'
    test case 2 unknown fctos:
        query: select fctos_to_devtype('Fortinet Winwin 7 Enterprise Edition, 32-bit Service Pack 1 (build 7601)');
        result: 'Unknown'

30. normalize_url(string arg1, string arg2, string arg3) returns string
    arg1: protocol string (protocol name in upper case such as 'HTTP' or 'HTTPS')
    arg2: hostname string
    arg3: path string (only the section before the question mark)
    arg4: dstport string (to be added)
    arg5: tdtype string (to be added)
    return: a formated url generated from the inputs (toLowerCase(protocol) + "://" + hostname + path (section before question mark))
    test case 1:
        query: select normalize_url('HTTPS', 'wiki.fortinet.com', '/twiki/bin/login/Main/OrderDinner?origurl=%2Ftwiki%2Fbin%2Fview%2FMain%2FOrderDinner');
        result: 'https://wiki.fortinet.com/twiki/bin/login/Main/OrderDinner'

31. ebtr_value(string arg1, int arg2, int arg3) returns int
    arg1: estimated browsing time range (ebtr) int4range array
    arg2: lower minimum of the time4range elements
    arg3: upper maximum of the time4range elements
    return: the ebtr value generated from the time4range array
    1. For one time range, when a range lower bound is less than lower minimum, take it as lower minimum.
       When a range upper bound is bigger than upper maximum, take it as upper maximum. Then, calculate the
       difference between upper bound and lower bound.
    2. For time range with double parathesis such as (1,2). The ebtr value is 'difference minus one'(2 - 1 - 1 = 0).
    3. For time range with double square brackets such as [1,2]. The ebtr value is 'difference plus one' (2 - 1 + 1 = 2).
    4. For tie range with one parentheses and one square bracket such as [1,2) or (1,2]. The ebtr value is 'difference' (2 - 1 = 1)
    5. The final result is the sum of these ebtr values.
    test case 1 time ranges with one parentheses and one square bracket :
        query: select ebtr_value('{"[3,7)","(8,10]"}',1,10);
        result: (7 - 3) + (10 - 8) = 6
    test case 2 time ranges with parenthesis:
        query: select ebtr_value('{"(1,2)","(2,3)"}',1,10);
        result: 0
    test case 3 time ranges with square brackets:
        query: select ebtr_value('{"[1,2]","[2,3]"}',1,10);
        result: 4
    TODO: Postgres has its own data type check. For time4range, [3,1] is invalid in postgres so an exception will be thrown.
          However, in Kudu, we don't have this check, so evil things will happen when input is '{"[100,1]","[10000,3]"}'.

32. fct_uuid_lt(string arg1, string arg2) returns boolean
    arg1: fctuid
    arg2: fctuid
    return: true if arg1 is smaller than arg2 by ascii values otherwise false (only compare the first 16 bytes)
    test case 1:
        query: select fct_uuid_lt('fct-12345678','fct-22345678');
        result: true
    test case 2:
        query: select fct_uuid_lt('fct-22345678','fct-12345678');
        result: false

33. fct_uuid_le(string arg1, string arg2) returns boolean
    arg1: fctuid
    arg2: fctuid
    return: true if arg1 is no bigger than arg2 by ascii values otherwise false (only compare the first 16 bytes)
    test case 1 arg1 is smaller:
        query: select fct_uuid_le('fct-12345678','fct-22345678');
        result: true
    test case 2 arg1 is bigger:
        query: select fct_uuid_le('fct-22345678','fct-12345678');
        result: false
    test case 3 arg1 and arg2 are equal:
        query: select fct_uuid_le('fct-22345678','fct-22345678');
        result: true

34. fct_uuid_eq(string arg1, string arg2) returns boolean
    arg1: fctuid
    arg2: fctuid
    return: true if arg1 equal to arg2 by ascii values otherwise false (only compare the first 16 bytes)
    test case 1 arg1 and arg2 are equal:
        query: select fct_uuid_eq('fct-22345678','fct-22345678');
        result: true
    test case 2 arg1 and arg2 are not equal:
        query: select fct_uuid_eq('fct-12345678','fct-22345678');
        result: false

35. fct_uuid_ge(string arg1, string arg2) returns boolean
    arg1: fctuid
    arg2: fctuid
    return: true if arg1 is no smaller than arg2 by ascii values otherwise false (only compare the first 16 bytes)
    test case 1 arg1 is bigger:
        query: select fct_uuid_ge('fct-32345678','fct-22345678');
        result: true
    test case 2 arg1 and arg2 are equal:
        query: select fct_uuid_ge('fct-12345678','fct-12345678');
        result: true
    test case 3 arg1 is smaller:
        query: select fct_uuid_ge('fct-12345678','fct-22345678');
        result: false

36. fct_uuid_gt(string arg1, string arg2) returns boolean
    arg1: fctuid
    arg2: fctuid
    return: true if arg1 is bigger than arg2 by ascii values otherwise false (only compare the first 16 bytes)
    test case 1 arg1 is bigger:
        query: select fct_uuid_gt('fct-32345678','fct-22345678');
        result: true
    test case 2 arg1 and arg2 are equal:
        query: select fct_uuid_gt('fct-12345678','fct-12345678');
        result: false
    test case 3 arg1 is smaller:
        query: select fct_uuid_gt('fct-12345678','fct-22345678');
        result: false

37. fct_uuid_ne(string arg1, string arg2) returns boolean
    arg1: fctuid
    arg2: fctuid
    return: true if arg1 is not equal to arg2 by ascii values otherwise false (only compare the first 16 bytes)
    test case 1 arg1 and arg2 are not equal:
        query: select fct_uuid_ne('fct-32345678','fct-22345678');
        result: true
    test case 2 arg1 and arg2 are equal:
        query: select fct_uuid_ne('fct-12345678','fct-12345678');
        result: false

38. fct_uuid_cmp(string arg1, string arg2) returns int
    arg1: fctuid
    arg2: fctuid
    return:
    1. result > 0 if arg1 is bigger arg2 by ascii values
    2. result = 0 if arg1 is equal to arg2 by ascii values
    3. result < 0 if arg1 is smaller than arg2 by ascii values
    test case 1 arg1 is bigger:
        query: select fct_uuid_cmp('fct-32345678','fct-22345678');
        result: 1
    test case 2 arg1 is smaller:
        query: select fct_uuid_cmp('fct-12345678','fct-22345678');
        result: -1
    test case 3 arg1 is equal to arg2:
        query: select fct_uuid_cmp('fct-12345678','fct-12345678');
        result: 0

39. fct_uuid_hash(string arg1) returns int
    arg1: fctuid
    return: an integer hash value calculated from the fctuid
    test case 1:
        query: select fct_uuid_hash('fct-12345678');
        result: 1531530053

```

## UDAs
To test UDAF, test tables should be created since they are aggregate functions that require "group by" syntax. Impala&Kudu as distributed database will split this query into sub-tasks among the impala daemon nodes.
Example test tables can be created through the following SQL commands.
```sql
    CREATE DATABASE IF NOT EXISTS db_public_kudu;
    DROP TABLE IF EXISTS db_public_kudu.transcript;
    CREATE TABLE db_public_kudu.transcript(
        ID string NOT NULL,
        Name string NOT NULL,
        Scores string NOT NULL,
        SchoolYear string NOT NULL,
    PRIMARY KEY (id)
    ) PARTITION BY HASH(id) PARTITIONS 4 STORED AS KUDU;
    INSERT INTO db_public_kudu.transcript VALUES ('1', 'Jack', '{70,80,90}', '1999'), ('2', 'Jack', '{80,90,90}', '2000'), ('3', 'Roger', '{10,20,30}', '1999'), ('4', 'Roger', '{30,40,50}', '2000');

    +----+-------+------------+------------+
    | id | name  | scores     | schoolyear |
    +----+-------+------------+------------+
    | 1  | Jack  | {70,80,90} | 1999       |
    | 4  | Roger | {30,40,50} | 2000       |
    | 3  | Roger | {10,20,30} | 1999       |
    | 2  | Jack  | {80,90,90} | 2000       |
    +----+-------+------------+------------+

    CREATE DATABASE IF NOT EXISTS db_public_kudu;
    DROP TABLE IF EXISTS db_public_kudu.userebtr;
    CREATE TABLE db_public_kudu.userebtr(
        ID string NOT NULL,
        Username string NOT NULL,
        Browsingtime string NOT NULL,
    PRIMARY KEY (id)
    ) PARTITION BY HASH(id) PARTITIONS 4 STORED AS KUDU;
    INSERT INTO db_public_kudu.userebtr VALUES ('1', 'Jack', '{"[3,9)","(8,10]"}'), ('2', 'Jack', '{"[10,20)","(22,80]"}'), ('3', 'Roger', '{"[113,150)","(160,200]"}'), ('4', 'Roger', '{"[180,220)","(300,400]"}');

    +----+----------+---------------------------+
    | id | username | browsingtime              |
    +----+----------+---------------------------+
    | 3  | Roger    | {"[113,150)","(160,200]"} |
    | 1  | Jack     | {"[3,9)","(8,10]"}        |
    | 4  | Roger    | {"[180,220)","(300,400]"} |
    | 2  | Jack     | {"[10,20)","(22,80]"}     |
    +----+----------+---------------------------+

```
```bash
1. array_agg(string arg1)
    arg1: table column that is an array
    return: aggregate the arrays into one big array group by another table columns
    test case 1:
        query: select array_agg(scores),name from transcript group by name;
        result:
               +----------------------------------+-------+
               | db_public_kudu.array_agg(scores) | name  |
               +----------------------------------+-------+
               | {30,40,50,10,20,30}              | Roger |
               | {70,80,90,80,90,90}              | Jack  |
               +----------------------------------+-------+

2. ebtr_agg_flat(string arg1)
    arg1: table column that is an int4range array
    return: aggregate the arrays into one big array group by another table columns
    1. The overlapping time ranges will be merged into one time range
    2. The result should be time4range array with no overlapping elements
    test case 1:
        query: select ebtr_agg_flat(browsingtime),username from userebtr group by username;
        result:
               +--------------------------------------------+----------+
               | db_public_kudu.ebtr_agg_flat(browsingtime) | username |
               +--------------------------------------------+----------+
               | {"[113,150)","(160,220)","(300,400]"}      | Roger    |
               | {"[3,20)","(22,80]"}                       | Jack     |
               +--------------------------------------------+----------+

3. loguids_agg(string arg1)
    arg1: an array with loguids
    return:
    1. aggregate loguids string into a string separated by delimiter ','.
    2. if any loguids is NULL, agg result will be NULL (Not implemented yet).
    limitations:
    1. Not sure what the loguids look like. For now, they are taken as array of integers.
    2. For now, basic function is the same as array_agg.
    test cases to be added in future.
```
