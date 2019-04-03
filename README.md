# How to run Model Data ETL (for testing purpose)

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
