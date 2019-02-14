# FAZBD UDF&UDAF For Impala


## Deploy UDF on HDFS

```bash
# 1. Obtain .so file and sql script through git
git clone git@dops-git.fortinet-us.com:FAZ-BD/platform.git

# 2. Change directory and locate libudffazbd.so file and udf_fazbd_cpp.sql script
cd fazudf

# 3. Configure HDFS user
export HADOOP_USER_NAME=hdfs

# 4. Delete old .so file on HDFS (safe mode may disable this so need access)
hadoop fs -rm /user/impala/udf/db_public_kudu.db/*faz*.so

# 5. Deploy libudffazbd.so file on HDFS
/usr/bin/hadoop fs -put libudffazbd.so /user/impala/udf/db_public_kudu.db

# 6. Load the UDFs into the db_public_kudu database with the sql script
impala-shell -f udf_fazbd_cpp.sql

# 7. Use the db_public_kudu database and try the UDFs with some sql queries
use db_public_kudu;
```

## Install UDF Development Env on CentOS

```bash
# 1. Install cmake and gcc
sudo yum install gcc-c++ cmake boost-devel

# 2. Install CDH repo
sudo yum -y install yum-utils

sudo yum-config-manager --add-repo http://archive.cloudera.com/cdh5/redhat/7/x86_64/cdh/cloudera-cdh5.repo

sudo yum-config-manager --enable cloudera-cdh5

# 3. Install impala UDF development environment
sudo yum install impala-udf-devel
```

## Compile source code for libudffazbd.so file with cmake

```bash
# 1. Change directory into udf source code
cd fazudf/src/main/c

# 2. Make .so file
cmake . && make

# 3. Browse the so file under build folder
ls build/*.so
```

## Compile source code for libudffazbd.so file with maven

```bash
# 1. Change directory into fazudf and locate pom.xml
cd fazudf

# 2. Build .so file with mvn
mvn package -Dmaven.test.skip=true

# 3. Browse the so file under build folder
ls src/main/c/build/*.so
```

# UDF&UDAF Spec and Test Cases

## limitations

FAZ's UDFs are for Postgres Database. Postgres supports abundant built-in data types and customized data type, but Impala does not support them. Hence, we have to use string to support these complex data types. For example, we have to use string to represent int4range, array, fct_uuid, inet and use int or double to represent smallint or numeric. 

## UDFs

```bash
1. threatweight_sum(string arg1, string arg2) returns int
    arg1: threatweights array
    arg2: threatcounts array
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
    arg2: threatlevels array
    arg3: threatcounts array
    arg4: threatweights array
    return: sum of (threatweight*threatcount) of input threatlevel
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
    arg1: threatlevels array
    return: max threatlevel in the input threatlevels array
    test case 1 threatlevels within range:
        query: select threatlevel_max('{1,2,3,4,1}');
        result: 4
    test case 2 threatlevels out of range:
        query: select threatlevel_max('{1,2,3,4,1111}');
        result: 1111 (not reasonable but follow faz design)

4. root_domain(string arg1) returns string
    arg1: hostname string
    return: domain name
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
        result: '2002:cc:ef::16' (only the ip part is kept)
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
    return: string same as input or NULL if 'N/A' or 'n/a'
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
    arg1: application group name (underscore separated)
    return: first token before first underscore
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
    arg1: email address string
    return: email user string
    test case 1 valid email address with '@':
        query: select email_user('dsshen@fortinet.com');
        result: 'dsshen' (token before '@')
    test case 2 invalid email address without '@':
        query: select email_user('dsshenfortinet.com');
        result: 'dsshenfortinet.com'

11. email_domain(string arg1) returns string
    arg1: email address string
    return: domain of the email address
    test case 1 valid email address with '@':
        query: select email_domain('dsshen@fortinet.com');
        result: 'fortinet.com' (token after '@')
    test case 2 invalid email address without '@':
        query: select email_domain('dsshenfortinet.com');
        result: empty output

12. date_time(bigint arg1) returns string
    arg1: UTC bigint
    return: human readable month/day/hour/minute string in local timezone in 'MM/DD hh24:mi' format
    test case 1 input of zero:
        query: select date_time(0);
        result: '01/01 08:00' (local timezone may affect this value)

13. tmtodate(bigint arg1) returns string
    arg1: UTC bigint
    return: human readable year/month/day string in local timezone in 'YYYY-MM-DD' format
    test case 1 input of zero:
        query: select tmtodate(0);
        result: '1970-01-01' (local timezone may affect this value)

14. tmtohour(bigint arg1) returns string
    arg1: UTC bigint
    return: human readable year/month/day/hour string in local timezone in 'YYYY-MM-DD HH24:00' format
    test case 1 input of zero:
        query: select tmtohour(0);
        result: '1970-01-01 08:00' (local timezone may affect this value)

15. logid_to_int(string arg1) returns int
    arg1: logid string
    return: cast the ending 6 digits to int
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
    arg1: inet string with or without subnet mask bits number
    return: the ip part of the inet string input
    limitations: inet type validation check is not implemented.
    test case 1 valid ip input:
        query: select ipstr('192.168.6.108');
        result: '192.168.6.108'
    test case 2 invalid ip input:
        query: select ipstr('192.168.6.256');
        result: NULL (different from FAZ since postgres will check inet data type automatically)
    test case 3 valid inet input with mask:
        query: select ipstr('192.168.31/24');
        result: '192.168.31.0'
    TODO issue: inet data type should be checked considering the ip part and the subnet mask. ipv6 should be also considered.

20. ip_subnet(string arg1) returns string
    arg1: inet string with or without subnet mask bits number
    return: ip subnet string
    TODO issue: just find this does not work for now. The algorithm is kind of complex.

21. vpn_trim(string arg1) returns string
    arg1: vpn string
    return: trim vpn string with regex '_[0-9]+$'
    test case 1 vpn string match the regex:
        query: select vpn_trim('aaaa_11111');
        result: 'aaaa'
    test case 2 vpn string not match regex:
        query: select vpn_trim('aaaa11111');
        result: 'aaaa11111'

22. fctver_trim(string arg1) returns string
    arg1: forticlient version string
    return: trim fctver string with regex '^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}'
    test case 1 valid input match the regex:
        query: select fctver_trim('111.111.11111111');
        result: '111.111.111'
    test case 2 invalid input that does not match the regex:
        query: select fctver_trim('11111.111.11111111');
        result: NULL

23. format_numeric_no_decimal(string arg1) returns string
    arg1: numeric string that is designed to be decimal(28,2)
    return: round the numeric number and return only the integer part
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












```

## UDAs

```bash
1. array_agg()

2. ebtr_agg_flat()

3. loguids_agg()
```
