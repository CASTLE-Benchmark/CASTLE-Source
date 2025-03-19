# Build the main CASTLE docker image builder
docker build --platform linux/amd64 -t castle-builder-ubuntu -f builder.Dockerfile .
