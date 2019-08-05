# Shcedule data retention/maintenance jobs (for dev purpose)

# 1. Prerequisite
We need an impala&kudu&datacatalog environment with default '_data_catalog' and 'db_log_public' databases that have the latest adom table schemas and UDFs.

If you do not have the mentioned environment, we can create one through the following steps.
## 1.1 Compile fazudf (Reference fazudf README)
Compile this C++ program first to generate the .so lib file.
This file will be used by data.catalog or impala docker to deploy UDFs.

## 1.2 Start impala&kudu database
Some existing docker files will deploy the fazudf .so file to hdfs for UDF installation.
```bash
# docker-compose-simulator_etl.yml is under 'platform/' folder
docker-compose -f docker-compose-simulator_etl.yml up impala
```

## 1.3 Start data.catalog
Compile first so that the udf creation script (called 'udf_fazbd_cpp.sql') is copied from the fazudf project.
Check src/main/resources to see whether there exists a 'udf_fazbd_cpp.sql' file after compilation.
Change 'server.port' param from '8080' to '8083' (or other ports that is free to use) in application.properties

Start data.catalog project through IDE. If this project is started successfully, you would be able to access the following link:
http://<IP>:8083/datacatalog/swagger-ui.html

# 2. Start db.management.task project

## 2.1 Modify application.properties under 'src/main/resources/'
Chnage 'ports' from '8080:8080' to '8081:8081' (or other ports that is free to use).

## 2.2 Start db.management.task through IDE
The APIs can be accessed through: http://<IP>:8081/task/swagger-ui.html

# 3. Start db.management.api project

## 3.1 Modify application.properties under 'src/main/resources/'
Set 'webclient.url.bd-task' to '<IP>:8081' (Remember we change this port to 8081 in db.management.task)
Set 'webclient.url.datacatalog' to '<IP>:8083' (Remember we change this port to 8083 in data.catalog setup)

## 3.2 Start db.management.api through IDE
The APIs can be accessed through: http://<IP>:8080/management/swagger-ui.html

# 4. Workflow to create&schedule a data retention/maintenance job

## 4.1 Access Web Page

```bash
# db.management.api UI Page
http://<IP>:8080/management/swagger-ui.html
```

## 4.2 Run management command to create new adom

```json
# /v1/data/adoms
param:
{
  "adomId": "adomWan2",
  "adomName": "adomWan2",
  "alertUsage": 0,
  "async": true,
  "backupPhases": {
    "coldPhase": {
      "enableColdPhase": true,
      "phaseAge": 0,
      "unit": "day"
    },
    "warmPhase": {
      "enableWarmPhase": true,
      "phaseAge": 0,
      "unit": "day"
    }
  },
  "description": "string",
  "deviceType": "fgt",
  "force": true,
  "maxAllowed": 2,
  "partitionPeriod": 1,
  "percentageDB": 1,
  "properties": {},
  "retentionArchive": 0,
  "retentionDB": 0,
  "retentionPeriod": 30,
  "tasks": {
    "maintenanceTask": {
      "cronExp": "0 */2 1 1 * ? *"
    },
    "rolloverTask": {
      "cronExp": "0 */2 1 1 * ? *"
    }
  },
  "tenantId": "",
  "unit": "day"
}

```
