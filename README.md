# Development Guide for FAZ-BD

All date time and timestamps mentioned in fazconnector and impala&kudu database adopt UTC time standard instead of local time zones.
Date time should follow "yyyy-mm-dd hh:mm:ss.SSS" format.

## 1 Prerequisites

### 1.1 [Install Docker](https://docs.docker.com/install/)

### 1.2 Setup FAZBD Docker Registry

#### Add 172.30.42.155  -> registry.fazbd.fortinet.com your host table:
```sh
echo "172.30.42.155 registry.fazbd.fortinet.com" >> /etc/hosts
```

#### Configuring your clients and projects to use FAZBD repos
#### For Linux Docker 

Add this into your /etc/docker/daemon.json

```json
{
   "insecure-registries":[
      "registry.fazbd.fortinet.com:8083"
   ]
}
```

Restart the Daemon

 ```sh
 #systemctl
 sudo systemctl restart docker
 
 #or, service
 sudo service docker restart
 ```
 
#### For Windows or MacOS user:

Click the docker icon -> Preference -> Daemon -> Basic -> Insecure registries: add `registry.fazbd.fortinet.com:8083`

Restart Docker Machine

#### 1.3 Docker login

```sh
docker login -u fazbd-read -p fortinet registry.fazbd.fortinet.com:8083
```

Then u will see:

```sh
"Login Succeeded"
```

Then run this for test:

```sh
docker pull registry.fazbd.fortinet.com:8083/hello-world:latest
```

## 2 FAZ-BD Docker Container

### 2.1 Start Services 

Docker images for the web services and its dependencies are provided with docker-compose-simulator_etl_qa.yml which can be used for integration.
It will start (simulator + ingestion server + kafka + etl + impala kudu + fazconnector) in docker compose. (It will take about several minutes)
Then the the web services will start with fazconnector container at `localhost:8080`. Server logs will be available via `docker logs {container_id}`

```bash
# start server
docker-compose -f docker-compose-simulator_etl_qa.yml up

# 2.ext Sometimes, handle for etl "Exit(1)"
# 2.ext.1 check all the contains up
docker ps -a
# 2.ext.2 if etl is the status of Exit(1), start etl again until it is up 
docker-compose -f docker-compose-simulator_etl.yml up etl

# stop server
docker-compose -f docker-compose-simulator_etl_qa.yml down
docker volume rm $(docker volume ls -qf dangling=true)
```

### 2.2 Known issue:
#### 2.2.1 System requires:

```
memory size > 16G

disk size for docker (/var/lib/docker) > 200G
```
#### 2.2.2 Impala docker is not stable. 
```
If it doesn't work any more , please run the command "down , clear volume and up". Make sure there is no "impala container" before up.
```

### 2.3 Access Swagger API Doc

```bash
# Swagger UI Page
http://localhost:8080/api/v1/swagger-ui.html
```

### 2.3 Simple usage
#### 2.3.1 Logview Query

```bash
# /logview/logsearch/start
adomName:adom1
param:
{
  "apiver": "3",
  "deviceModel": "fgt",
  "filter": [[["app","==","DNS"],"||",["srcport","!=","53440"]],"&&",[["rcvdbyte","in","77","126"],"&&",["dstepid","==","3261"]]],
  "logType": "traffic",
  "sortBy": {
    "field": "id",
    "order": "desc"
  },
  "timeOrder": "desc",
  "timeRange": {
    "end": "2019-04-18 20:45:37",
    "start": "2019-04-09 20:35:37"
  }
}

Description:
The "filter":
    [[["app","==","DNS"],"||",["srcport","!=","53440"]],"&&",[["rcvdbyte","in","77","126"],"&&",["dstepid","==","3261"]]]
means:
    where (app='DNS' or srcport!=53440) and (rcvdbyte in (77,126) and dstepid='3261')
  
Response body:
{
  "status": {
    "code": 0
  },
  "result": {
    "taskId": "93d79528-4908-4205-82a9-616cb253db15"
  }
}

```

```bash
# /logview/logsearch/fetch
adomName:adom1
taskId: start command response e.g. daea6edf-46cd-4e78-9a63-97df0d4509ef
param:
{
  "apiver": "3",
  "limit": 50,
  "offset": 0
}

Response body:
{
  "status": {
    "message": "Complete",
    "code": 0
  },
  "result": {
    "taskId": "93d79528-4908-4205-82a9-616cb253db15",
    "data": [
      {
        "srcuuid": "35f2ab5c-ef55-51e8-d67e-f8a5618246ab",
        "dvid": "21000",
        "tdtype": null,
        ......
        "user": null,
        "rcvdpkt": null
      },
	  ......
    ],
    "returnLines": 50,
    "percentage": 100
  }
}

```

#### 2.3.2 QueryEngine API

```bash
# /queryengine/start
adomName:adom1
sql:
select `id`,`itime`,`bid`,`dvid`,`dtime`,`type`,`subtype`,`level`,`action`,`utmaction`,`vd` 
from `db_log_public`.`adom1_fgt_traffic` where `itime` >= '2019-04-09 20:35:37' and `itime` <= '2019-04-18 20:45:37' order by `itime` desc

Response Body:
{
  "status": {
    "code": 0
  },
  "result": {
    "taskId": "38adb03a-ad00-4a54-98f7-baae4c466f73"
  }
}

```

```bash
# /queryengine/fetch
adomName:adom1
taskId: start command response e.g. daea6edf-46cd-4e78-9a63-97df0d4509ef
param:
{
  "limit": 50,
  "offset": 0
}

Response Body:
{
  "status": {
    "message": "Complete",
    "code": 0
  },
  "result": {
    "taskId": "c8c80d6d-4994-4594-b7f2-6da0d08cb706",
    "data": [
      {
        "utmaction": null,
        "dvid": "21000",
        "subtype": "forward",
        "level": "warning",
        "dtime": 1555422718245,
        "action": "ip-conn",
        "id": "750311d6-7e5b-4053-a9cc-e4cc316433a9",
        "itime": 1555422718245,
        "bid": "6431321",
        "type": "traffic",
        "vd": "root"
      },
	  ......
    ],
    "returnLines": 50,
    "percentage": 100
  }
}

```

#### 2.3.3 DBEngine API
 * Adom name paramter is optional
 * For global table operation, just leave adom name as blank
 * For ADOM table operation, adom name must have value
 
```bash
# /dbengine/execute
## execute under global space
adomName:keep adomName as blank
sql:
CREATE TABLE ${devtable} (
    dvid integer NOT NULL,
    devid string NOT NULL,
    vd string,
    csf string,
    devname string,
    ctime integer DEFAULT 0 NOT NULL,
    lastseen integer DEFAULT 0 NOT NULL,
    primary key (dvid)
)

## execute under specific adom
adomName:adom1
sql:
CREATE TABLE ${devtable} (
    dvid integer NOT NULL,
    devid string NOT NULL,
    vd string,
    csf string,
    devname string,
    ctime integer DEFAULT 0 NOT NULL,
    lastseen integer DEFAULT 0 NOT NULL,
    primary key (dvid)
)

Response Body:
{
  "status": {
    "code": 0
  },
  "result": 0
}

```

```bash
# /dbengine/query
adomName:adom1
sql:
select `id`,`itime`,`bid`,`dvid`,`dtime`,`type`,`subtype`,`level`,`action`,`utmaction`,`vd` 
from ${fgt_traffic} 
where `itime` >= '2019-04-09 20:35:37' and `itime` <= '2019-04-18 20:45:37' order by `itime` desc
limit 2

Response Body:
{
  "status": {
    "code": 0
  },
  "result": {
    "data": [
      {
        "id": 3369361375484136400,
        "itime": "2019-04-16T13:52:32.601+0000",
        "bid": "6431321",
        "dvid": "21000",
        "dtime": "2019-04-16T13:52:32.601+0000",
        "type": "traffic",
        "subtype": "forward",
        "level": "warning",
        "action": "ip-conn",
        "utmaction": null,
        "vd": "root"
      },
      {
        "id": 5325441086036819000,
        "itime": "2019-04-16T13:50:47.162+0000",
        "bid": "6431321",
        "dvid": "21000",
        "dtime": "2019-04-16T13:50:47.162+0000",
        "type": "traffic",
        "subtype": "forward",
        "level": "notice",
        "action": "accept",
        "utmaction": "allow",
        "vd": "root"
      }
    ]
  }
}
```

```bash
# /dbengine/batch
adomName:adom1
sql:
["DROP TABLE IF EXISTS ${devtable}", 
 "CREATE TABLE ${devtable} ( dvid integer NOT NULL, devid string NOT NULL, vd string, csf string, devname string, ctime integer DEFAULT 0 NOT NULL, lastseen integer DEFAULT 0 NOT NULL, primary key (dvid) )",
 "DESCRIBE ${devtable}"]

Response Body:
{
  "status": {
    "code": 0
  },
  "result": [
    {
      "name": "dvid",
      "type": "int",
      "comment": "",
      "primary_key": "true",
      "nullable": "false",
      "default_value": "",
      "encoding": "AUTO_ENCODING",
      "compression": "DEFAULT_COMPRESSION",
      "block_size": "0"
    },
    ...
    {
      "name": "lastseen",
      "type": "int",
      "comment": "",
      "primary_key": "false",
      "nullable": "false",
      "default_value": "0",
      "encoding": "AUTO_ENCODING",
      "compression": "DEFAULT_COMPRESSION",
      "block_size": "0"
    }
  ]
}

```

### 2.4 Demo Data Supported Range

The demo data within the embedded database support the following types and time ranges:

```
| deviceModel | logType   | timeRange                                                   |
|-------------|-----------|-------------------------------------------------------------|
| "fgt"       | "traffic" | "end": "2019-04-15 16:36:24","start": "2019-04-16 21:51:58" |
| "fgt"       | "ips"     | "end": "2019-04-15 16:35:56","start": "2019-04-16 12:28:11" |
| "fch"       | "dlp"     | "end": "2019-04-15 16:36:04","start": "2019-04-16 15:17:13" |
| "fwb"       | "attack"  | "end": "2019-04-15 16:36:29","start": "2019-04-16 23:12:23" |
| "fwb"       | "event"   | "end": "2019-04-15 16:35:22","start": "2019-04-16 05:04:37" |
