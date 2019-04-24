# Model Data ETL Test
## Environment Setup

```bash
1.1 docker & docker-compose (if you have access to the source code)
We use docker to do the test. The easiest way is to follow the README doc to start the Ingestion Server, Model Data ETL and Impala-Kudu together using docker-compose.

# Change directory to /model.data.etl if you have all source code
## Install docker, docker-compose and maven first on centos
## Use maven to compile
mvn clean install

## Start ingestion server, model data etl, and impala-kudu with docker
docker-compose up

## Stop all
docker-compose down

1.2	Setup Kafka, Zookeeper, Ingestion Server, and Model Data ETL on real PC
# install and start Kafka and zookeeper
# deploy and start ingestion server jar
# deploy and start model data etl jar
# install impala and kudu
I can provide all the necessary jars to get the installation done. In addition, I can help with the setup process of the ingestion server and model.data.etl if they fail to connect with each other with the default config.
```

## DB Initialization (Create these test tables in the Kudu DB)
With impala-shell, you can initialize the database with a script (impala-shell –f myscript.sh) containing these sql sentences or just type these sql sentences in impala-shell.

```sql
CREATE DATABASE IF NOT EXISTS public;
DROP TABLE IF EXISTS public.devtable;
CREATE TABLE public.devtable (
    dvid integer NOT NULL,
    devid string NOT NULL,
    vd string,
    csf string,
    devname string,
    ctime integer DEFAULT 0 NOT NULL,
    lastseen integer DEFAULT 0 NOT NULL,
    primary key (dvid)
) PARTITION BY HASH PARTITIONS 16 STORED AS KUDU TBLPROPERTIES('kudu.num_tablet_replicas' = '1');

DROP TABLE IF EXISTS public.endpoints;
CREATE TABLE public.endpoints (
    epid integer NOT NULL,
    srctype string NOT NULL,
    adomoid bigint DEFAULT 0 NOT NULL,
    fctuid string,
    mac string,
    epip string,
    dvid integer,
    lastseen integer,
    srcname string NOT NULL,
    epname string,
    hostname string,
    epdevtype string,
    osname string,
    osversion string,
    firstseen integer,
    primary key (epid)
) PARTITION BY HASH PARTITIONS 16 STORED AS KUDU TBLPROPERTIES('kudu.num_tablet_replicas' = '1');

DROP TABLE IF EXISTS public.app_mdata;
CREATE TABLE public.app_mdata (
    id integer NOT NULL,
    name string NOT NULL,
    risk string,
    app_cat string,
    app_cat_id string,
    vendor string,
    technology string,
    behavior string,
    primary key (id)
) PARTITION BY HASH PARTITIONS 16 STORED AS KUDU TBLPROPERTIES('kudu.num_tablet_replicas' = '1');
```

## Test Cases (Compare those in Kudu DB with expected results)
We use junit test and simulator to send requests. Not sure whether QA can use the same way to send requests or you have other ways to do this.

```bash
3.1	Send good "Create" batch operations to ingestion server
Insert some records into the Kudu DB and compare those in DB with expected results.

3.2	Send good "Update" batch operations to ingestion server
Update some existing records and compare the updates with expected results.

3.3	Send good "Delete" batch operations to ingestion server
Delete specified records that already exist in DB and check whether those are deleted successfully.

3.4 Send model data requests with bad batch operations
For example, send batch operation requests with unknown column name to a table. The records can be successfully inserted but with a null value rather than result in an insert failure.
Another example might be update or delete non-existing records or create when record already exists or insert empty record, etc.

3.5 Trigger retry consumer or error consumer with exceptions
(1) Stop impala&kudu database will result in "java.net.ConnectException"
This should trigger retry consumer.

(2) Send invalid sql queries will result in "java.sql.SQLException"
This should trigger error consumer but not sure how to reproduce this.
```
