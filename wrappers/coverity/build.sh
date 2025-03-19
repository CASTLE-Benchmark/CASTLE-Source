cp ../../dataset.json dataset.json
docker build -t castle-wrapper-coverity .
docker run --rm -it castle-wrapper-coverity
