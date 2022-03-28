FROM ubuntu:20.04
#ENV PYTHONUNBUFFERED 1
#ENV DEBIAN_FRONTEND=noninteractive
ENV STM32CUBEIDE_VERSION 1.8.0
ENV PATH="${PATH}:/opt/st/stm32cubeide_${STM32CUBEIDE_VERSION}"

RUN apt-get update && apt-get install -y \
    unzip \
    zip \
    curl \
    expect 

# Download from ST website
RUN curl --insecure -o /tmp/stm32cubeide-installer.sh.zip "https://www.st.com/content/ccc/resource/technical/software/sw_development_suite/group0/dc/49/8b/46/40/16/4e/20/stm32cubeide_lnx/files/st-stm32cubeide_1.8.0_11526_20211125_0815_amd64.sh.zip/jcr:content/translations/en.st-stm32cubeide_1.8.0_11526_20211125_0815_amd64.sh.zip" && \
    unzip -p /tmp/stm32cubeide-installer.sh.zip > /tmp/stm32cubeide-installer.sh && rm /tmp/stm32cubeide-installer.sh.zip

COPY installer.sh .

# Using expect to install STM32 Cube IDE automatically.
RUN chmod +x installer.sh /tmp/stm32cubeide-installer.sh && \
    ./installer.sh && \
    rm /tmp/stm32cubeide-installer.sh installer.sh

# Install dependencies
RUN apt-get -y -f install

WORKDIR /workspace