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
All commands below should be run under the project root (model.data.etl/)

## Option 1
**All (kafka + model data etl) in docker compose**

Make sure you have aws cli preinstalled
aws default region is us-west-1, default output is None

```bash
# 1. Setup env
aws configure
eval $(aws ecr get-login --no-include-email | sed 's|https://||')

# 2. Build project and images
mvn clean install

# 3. Start all
docker-compose up --build

# 4. Stop all
docker-compose down

```

## Option 2
**kafka docker + model data etl on host**

```bash
# build latest
mvn clean install

# start kafka and zookeeper
docker-compose -f docker-compose-kafka_only.yml up -d

# start model data etl
java -jar target/com.fortidata.model.data.etl-0.0.1-SNAPSHOT.jar

# stop kafka and zookeeper
docker-compose down

```

## Create test tables in Impala&Kudu database

```bash
# locate "src/test/resources/createTables.sql" and use impala-shell to create the tables
impala-shell -f createTables.sql

```

## Kafka producer (run functional test)
All commands below should be run when both kafka and model data etl are running.

```bash
# update "test_producer.properties" (model.data.etl/src/main/test/resources)
bootstrap.servers=localhost:9092 (change to your own IP and port)
topic=model_data (change to your own topic name)

# run the test functions in BatchOpsProducerApp.java (model.data.etl/src/main/test/producer)
# or run with maven under root directory as follows
mvn -Dtest=com.fortidata.model.data.etl.test.producer.BatchOpsProducerApp#testCreate test

```
