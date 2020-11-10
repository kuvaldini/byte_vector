FROM gitpod/workspace-full

RUN sudo apt-get update && \
    sudo env DEBIAN_FRONTEND=noninteractive \
        apt-get install -yy --no-install-recommends \
        gcc g++ make cmake curl git ca-certificates ssh
RUN sudo apt-get clean -y
