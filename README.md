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

## Kafka producer (run junit test)
All commands below should be run when both kafka and model data etl are up.

```bash
1. Send 20 create records
mvn -Dtest=com.fortidata.model.data.etl.test.producer.BatchOpsProducerApp#testCreate test

2. Update the sent 20 records
mvn -Dtest=com.fortidata.model.data.etl.test.producer.BatchOpsProducerApp#testUpdate test

3. Delete the updated 20 records
mvn -Dtest=com.fortidata.model.data.etl.test.producer.BatchOpsProducerApp#testDelete test

```
