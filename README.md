# bd.management.api

## 1. Swagger UI
http://localhost:8080/management/swagger-ui.html

## 2. Design Doc
https://docs.google.com/document/d/1dDbqMyjhG5sDIeDypeglHqUk9oCMSi0UuhRrmSUPiKw

## 3. Service Configuration
https://docs.google.com/spreadsheets/d/153Cy__BD6K6toNyGmMWwkoLPahjySkcJ4EfEYuaOnRA

## 4. How to run

### 4.1 If you would like to work on api and task project
* Start docker compose file: ./demo/docker-compose-minidev.yml
* Change bd.management.task port to 8081 in application.properties file ("server.port = 8081")
* Change application.properties file in bd.management.api project: webclient.url.bd-task = localhost:8081
* Start bd.management.task project
* Start bd.management.api project

### 4.2 If you would like to work on data retention job
* Follow the above steps
* Change application.properties file "webclient.url.datacatalog =" to point to datacatalog url
* You can call DataApi::createDataRetentionJob and DataApi::scheduleDataRetentionJob to start a data retention job
* Job code is under bd.management.task project DataRetentionJobHandler.java

## 5. Workflow to create retention&maintenance jobs

### 5.1 Access Task Web Page

```bash
# db.management.api UI Page
http://<IP>:8080/management/swagger-ui.html
```

### 5.2 Run management command to create new adom (data retention&maintenance jobs are created for the new adom)

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
  "result": true,
  "details": "action_queued",
  "message": ""
}

```

## 6. Schedule retention&maintenance jobs

### 6.1 Access Task Web Page

```bash
# db.management.task UI Page
http://<IP>:8081/task/swagger-ui.html
```

### 6.2 Run task command to list the newly created jobs
Remember the jobName in the response. We will use them later.

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
    "jobName": "data-retention-adomWan2",
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
    "jobName": "data-maintenance-adomWan2",
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

### 6.3 Run schedule command to list created jobs
With the jobName in step 6.2, we can set the param here.

```json
# /v1/schedule
param:
{
  "jobName": "data-maintenance-adomWan2",
  "runtimeParams": {}
}

Response Body:
{
  "instanceId": "",
  "firstFireTime": "2019-08-05T19:00:00.000+0000"
}

```

### 6.4 Run unschedule command to list created jobs
Use the same jobName as that in step 6.3.

```json
# /v1/jobs/{job_name}/unschedule
param:
{
  "jobName": "data-maintenance-adomWan2",
  "runtimeParams": {}
}

Response Body: true

```
