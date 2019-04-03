# How to run Model Data ETL (for testing purpose)

## Update application.properties file (model.data.etl/src/main/resources)

```bash
# change kafka servers to your IP and port such as "localhost:9092"
spring.kafka.consumer.bootstrap-servers=<ip>:<port>

# change consumer topic to your topic such as "model_data"
spring.kafka.consumer.modeldata.topic=<topicName>

# change impala connection url to your impala url such as "jdbc:impala://192.168.6.105:21051"
modeldata.datasource.hikari.jdbc-url=<impala_url>

```

## Start kafka and model data etl
**kafka docker + model data etl on host**

All commands below should be run under the project root (model.data.etl/)

```bash
# build latest
mvn clean install

# start all
docker-compose -f docker-compose-kafka_only.yml up -d
java -jar target/com.fortidata.model.data.etl-0.0.1-SNAPSHOT.jar

# stop kafka
docker-compose down

```

## Create test tables in Impala&Kudu database

```sql
# create test database "public" and test table "devtable" in impala-shell
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
) PARTITION BY HASH(dvid) PARTITIONS 2 STORED AS KUDU;

```

## Kafka producer (run functional test)
All commands below should be run when both kafka and model data etl are running.

```bash
# update "test_producer.properties" (model.data.etl/src/main/test/resources)
bootstrap.servers=localhost:9092 (change to your own IP)
topic=model_data (change to your own topic name)

# run the test functions in BatchOpsProducerApp.java (model.data.etl/src/main/test/producer)

```
