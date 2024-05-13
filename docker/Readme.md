# Docker image

Docker image to test the implementations of the turbotouchpredictor.

## Create the image

```
docker build -t turbotouchpredictor .
```

## Run

In the parent folder:
```
docker run -i -t -d --name=turbotouchpredictor_con --mount type=bind,source=.,target=/mnt/turbotouchpredictor turbotouchpredictor
```

## Connect

```
docker exec -it turbotouchpredictor_con /bin/bash
cd /mnt/turbotouchpredictor
make
```