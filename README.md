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

## UDFs

#### threatweight_sum()

#### threatweight_level_sum()

#### threatlevel_max()


## UDAs

#### array_agg()

#### ebtr_agg_flat()

#### loguids_agg()
