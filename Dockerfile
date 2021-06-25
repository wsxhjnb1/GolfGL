FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=nonintercative

RUN apt update && \
	apt install -y g++ cmake git build-essential libgl1-mesa-dev libsoil-dev libglm-dev libassimp-dev libglew-dev \
	libglfw3-dev libxinerama-dev libxcursor-dev libxi-dev mesa-common-dev mesa-utils libxxf86vm-dev libfreetype6-dev
	
COPY . /usr/src/docker-image

WORKDIR /usr/src/docker-image	

RUN mkdir build && cd build && cmake .. && make



WORKDIR /usr/src/docker-image/bin/GolfGL	

ENTRYPOINT [ "./GolfGL" ]	


