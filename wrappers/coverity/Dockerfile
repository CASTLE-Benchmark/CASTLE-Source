FROM ubuntu

WORKDIR /app
COPY polaris /bin/polaris
RUN chmod +x /bin/polaris
COPY .token .token
COPY run.py run.py
COPY dataset.json dataset.json

RUN apt-get update && apt-get install -y ca-certificates build-essential git python3

ENV POLARIS_ACCESS_TOKEN_FILE=.token
#RUN polaris install --server-url https://carlzeiss.polaris.synopsys.com

CMD ["/bin/bash"]