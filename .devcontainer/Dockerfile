FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update \
  && apt-get install -y tzdata \
  gdb \
  xorg \
  libxext-dev \
  libbsd-dev \
  xdotool \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

ENV TZ=Asia/Tokyo
