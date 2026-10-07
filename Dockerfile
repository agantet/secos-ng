FROM debian:12

# Install utils and build dependencies
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y --no-install-recommends \
    tzdata \
    ca-certificates \
    wget \
    curl \
    vim \
    nano \
    qemu-system-x86 \
    qemu-kvm \
    gcc-multilib \
    make \
    git

# Install debug dependencies
RUN apt install -y --no-install-recommends \
    python3 \
    python3-pip \
    python3-venv \
    gdb \
    gdb-multiarch

# Setup gdb + gef
WORKDIR /root
RUN wget -q https://raw.githubusercontent.com/bata24/gef/dev/install.sh -O- | sed -e 's/pip3 install/pip3 install --break-system-packages/g' | sh

# Clone the secos-ng repository
RUN git clone https://github.com/agantet/secos-ng.git && \
    git config --global --add safe.directory /root/secos-ng

# Add a nicer prompt
RUN echo 'PS1="🐳 \\[\\033[1;36m\]secos-ng-\\h \\[\033[1;34m\\]\\W\\[\\033[0;35m\\] \\[\\033[1;36m\\]# \\[\\033[0m\\]"' > .bash_profile

ENTRYPOINT ["/bin/bash", "-l"]
CMD []
