FROM ubuntu:20.04
#ENV PYTHONUNBUFFERED 1
#ENV DEBIAN_FRONTEND=noninteractive
ENV STM32CUBEIDE_VERSION 1.9.0
ENV PATH="${PATH}:/opt/st/stm32cubeide_${STM32CUBEIDE_VERSION}"

RUN apt-get update && apt-get install -y \
    unzip \
    zip \
    curl \
    expect 

# Download from ST website
RUN curl --insecure -o /tmp/stm32cubeide-installer.sh.zip "https://gitlab.com/masitech/stm32-ide/-/package_files/33690434/download" && \
    unzip -p /tmp/stm32cubeide-installer.sh.zip > /tmp/stm32cubeide-installer.sh && rm /tmp/stm32cubeide-installer.sh.zip

COPY installer.sh .

# Using expect to install STM32 Cube IDE automatically.
RUN chmod +x installer.sh /tmp/stm32cubeide-installer.sh && \
    ./installer.sh && \
    rm /tmp/stm32cubeide-installer.sh installer.sh

# Install dependencies
RUN apt-get -y -f install

WORKDIR /workspace