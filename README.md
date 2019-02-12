# Deploy UDF&UDAF on Impala

##### Obtain .so file and sql script through svn or git  
svn checkout https://scm-yvr-mirror.fortinet.com/svn/global/FortiMonitor_ThirdParty/trunk/doc/modules/faz/sqoop/udf  

##### Find .so file and sql script that are named as follows.  
libudffazbd.so  
udf_fazbd_cpp.sql  

##### Configure HDFS user  
export HADOOP_USER_NAME=hdfs  

##### Deploy the .so file on HDFS  
/usr/bin/hadoop fs -put libudffazbd.so /user/impala/udf/db_public_kudu.db  

##### Load the UDFs into the db_public_kudu database with the sql script  
impala-shell -p udf_fazbd_cpp.sql  

##### Use the db_public_kudu database and try the UDFs with whatever sql queries
use db_public_kudu;  
