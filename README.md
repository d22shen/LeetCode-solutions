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

FAZ's UDFs are for Postgres Database. Postgres supports abundant built-in data types and customized data type, but Impala does not support them. Hence, we have to use string to support these complex data types. For example, we have to use string to represent int4range, array, fct_uuid, inet and use int or double to represent smallint or numeric in Postgres DB.

## UDFs

```bash
1. threatweight_sum(string arg1, string arg2) returns int
   arg1: threatweights that is int array like '{1,1,1,1,1}'
   arg2: threatcounts that is int array like '{1,2,3,4,5}'
   return: sum of (threatweight*threatcount) like 1*1+1*2+1*3+1*4+1*5 = 15
   limitations:
      1. arg1 and arg2 should be arrays with same number of elements. Otherwise, zero would be returned.

2. threatweight_level_sum(int arg1, string arg2, string arg3, string arg4)
   arg1: threatlevel that is int like 1
   arg2: threatlevels that is int array like '{1,2,3,4,1}'
   arg3: threatcounts that is int array like '{1,2,3,4,5}'
   arg4: threatweights that is int array like '{1,1,1,1,1}'
   return: sum of (threatweight*threatcount) for input threatlevel like 1*1+1*5=6 when input threatlevel is 1
   limittations:
      1. arg1, arg2 and arg3 should be arrays with same number of elements. Otherwise, zero would be returned.
      2. threatlevel range from 1 to 4 inclusive

3. threatlevel_max()
```

## UDAs

```bash
1. array_agg()

2. ebtr_agg_flat()

3. loguids_agg()
```
