# Impala UDF&UDAF


## Deployment

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
