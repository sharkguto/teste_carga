#FROM python:3.7-alpine
FROM python:3.7-slim

# RUN apk add --update \
#     curl \
#     bash \
#     && rm -rf /var/cache/apk/*

RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    curl \
    unzip \
    gcc \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /opt/wsd
WORKDIR /opt/wsd

RUN curl -L -O "https://github.com/joewalnes/websocketd/releases/download/v0.3.0/websocketd-0.3.0-linux_amd64.zip"
RUN unzip websocketd-0.3.0-linux_amd64.zip
RUN chmod +x websocketd
RUN cp websocketd /usr/local/bin/websocketd
RUN echo 'root:x:0:0:root:/root:/bin/bash\n' >> /etc/passwd
ENV PATH="/opt/wsd:${PATH}"

COPY src/ /opt/wsd
RUN chmod +x run.sh

COPY requirements.txt .

RUN python3 -m pip install -r requirements.txt

#ENTRYPOINT [ "websocketd", "--port=80", "sh", "run.sh" ]

#CMD tail -f /dev/null

ENTRYPOINT [ "sh","run.sh" ]