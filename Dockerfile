FROM quay.io/pypa/manylinux_2_28_x86_64
ADD . /src
RUN mkdir /dist
WORKDIR /src
RUN sed -i 's/LOCAL_BUILD/DOCKER_BUILD/' src/point_in_polygon/point_in_polygon.cpp
ENTRYPOINT [ "bash", "scripts/build--docker.sh" ]
