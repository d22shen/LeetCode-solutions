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
Chnage 'server.port' from '8080' to '8081' (or other ports that is free to use).

## 2.2 Start db.management.task through IDE
The APIs can be accessed through: http://<IP>:8081/task/swagger-ui.html

# 3. Start db.management.api project

## 3.1 Modify application.properties under 'src/main/resources/'
Set 'webclient.url.bd-task' to '<IP>:8081' (Remember we change this port to 8081 in db.management.task)
Set 'webclient.url.datacatalog' to '<IP>:8083' (Remember we change this port to 8083 in data.catalog setup)

## 3.2 Start db.management.api through IDE
The APIs can be accessed through: http://<IP>:8080/management/swagger-ui.html

# 4. Create retention&maintenance jobs

## 4.1 Access Task Web Page

```bash
# db.management.api UI Page
http://<IP>:8080/management/swagger-ui.html
```

## 4.2 Run management command to create new adom (data retention&maintenance jobs are created for the new adom)

```json
# /v1/data/adoms
Param:
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

Response Body:
{
  "result": true, // true means successfully executed
  "details": "action_queued",
  "message": ""
}

```

# 5. Schedule retention&maintenance jobs

## 5.1 Access Task Web Page

```bash
# db.management.task UI Page
http://<IP>:8081/task/swagger-ui.html
```

## 5.2 Run task command to list the newly created jobs

```json
# /v1/jobs
Param: no params needed

Response Body:
[
  {
    "createdTimestamp": 1564989903382,
    "updatedTimestamp": 1564989903382,
    "startTime": null,
    "endTime": null,
    "cronExp": "0 0 1 1/1 * ? *",
    "scheduled": false,
    "jobName": "data-retention-adomWan2", // new data retention jobName
    "jobType": "data-retention",
    "jobParams": {
      "unit": "day",
      "job_label": "adomWan2",
      "adom_id": "adomWan2",
      "retention_period": 30
    },
    "notificationType": null,
    "notificationData": null,
    "cronJob": true
  },
  {
    "createdTimestamp": 1564989903430,
    "updatedTimestamp": 1564989903430,
    "startTime": null,
    "endTime": null,
    "cronExp": "0 */5 3 1/1 * ? *",
    "scheduled": false,
    "jobName": "data-maintenance-adomWan2", // new data maintenance jobName
    "jobType": "data-maintenance",
    "jobParams": {
      "job_label": "adomWan2",
      "adom_id": "adomWan2"
    },
    "notificationType": null,
    "notificationData": null,
    "cronJob": true
  }
]

```

## 5.3 Run schedule command to list created jobs

```json
# /v1/schedule
param:
{
  "jobName": "data-maintenance-adomWan2", // get this jobName through step 5.2
  "runtimeParams": {}
}

Response Body:
{
  "instanceId": "",
  "firstFireTime": "2019-08-05T19:00:00.000+0000"
}

```

## 5.4 Run unschedule command to list created jobs

```json
# /v1/jobs/{job_name}/unschedule
param:
{
  "jobName": "data-maintenance-adomWan2", // the same jobName in step 5.3
  "runtimeParams": {}
}

Response Body: true

```
