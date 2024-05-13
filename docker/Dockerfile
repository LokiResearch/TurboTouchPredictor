FROM debian:bookworm-slim

ARG BUILD_DATE

LABEL maintainer="Géry Casiez <gery.casiez@univ-lille.fr>" \
	  description="Docker image for running tests on the turbotouch predictor" \
	  version="1.0" \
	  org.label-schema.build-date=$BUILD_DATE

RUN apt-get update && apt-get install -y \
	sudo \
	python3 \
	python3-pip \
	qt6-base-dev \
	g++ \
	openjdk-17-jdk \
	nodejs \
	npm \
	node-typescript

RUN pip install pandas argparse --break-system-packages
