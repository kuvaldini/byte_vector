FROM gitpod/workspace-full

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -yy --no-install-recommends \
        gcc g++ make cmake curl git ca-certificates ssh
RUN apt-get clean -y
