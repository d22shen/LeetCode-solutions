# Impala UDF&UDAF


## Deploy UDF on HDFS

```bash
# 1. Obtain .so file and sql script through svn or git
svn checkout https://scm-yvr-mirror.fortinet.com/svn/global/FortiMonitor_ThirdParty/trunk/doc/modules/faz/sqoop/udf

# 2. Locate libudffazbd.so file and udf_fazbd_cpp.sql script.

# 3. Configure HDFS user
export HADOOP_USER_NAME=hdfs

# 4. Deploy libudffazbd.so file on HDFS
/usr/bin/hadoop fs -put libudffazbd.so /user/impala/udf/db_public_kudu.db

# 5. Load the UDFs into the db_public_kudu database with the sql script
impala-shell -p udf_fazbd_cpp.sql

# 6. Use the db_public_kudu database and try the UDFs with some sql queries
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

## Compile source code for .so file

```bash
# 1. Change directory into udf source code and build .so file
cmake . && make

# 2. Browse the so file under build folder
ls build/*.so
```
