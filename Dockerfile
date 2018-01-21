FROM amazonlinux:2017.09.0.20170930

RUN yum -e 0 -y install gcc72.x86_64
RUN yum -e 0 -y install gcc72-c++.x86_64
RUN yum -e 0 -y install cmake
RUN yum -e 0 -y install python27-devel
RUN yum -e 0 -y install zip findutils

ADD build.sh /usr/local/bin/build.sh
RUN chmod +x /usr/local/bin/build.sh

ADD runtime /usr/local/runtime
RUN mkdir /usr/local/runtime/dist

RUN cd /usr/local/runtime/python && python -m compileall -d runtime . && cp *.pyc ../dist

RUN cd /usr/local/runtime/cpp/vendor/json && make

RUN cd /usr/local/runtime/cpp && cmake . && make && cp *.so* ../dist