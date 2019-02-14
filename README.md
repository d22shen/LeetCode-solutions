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
        result: 'abc.am.com'
    known issue: test case 2 succeeds by chance because the helper function 'lookup_domain_info' is not implemented yet.
        'lookup_domain_info' is to check whether some domain is in top domain list. FAZBD should also have this helper function.

5. nullifna(string arg1) returns string
    arg1: string
    return: string
    test case 1 string with is not 'N/A' or 'n/a'
        query: select nullifna('aaaaa');
        result: 'aaaaa'
    test case 2 'N/A':
        query: select nullifna('N/A');
        result: NULL
    test case 2 'n/a':
        query: select nullifna('n/a');
        result: NULL




```

## UDAs

```bash
1. array_agg()

2. ebtr_agg_flat()

3. loguids_agg()
```
