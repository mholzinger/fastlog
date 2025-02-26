# Use the official Rocky Linux 9 image as the base
FROM rockylinux:9

# Set environment variables
ENV HOME /root
WORKDIR $HOME

# Install necessary dependencies
RUN dnf install -y \
    gcc \
    make \
    git \
    glibc-devel \
    rsyslog \
    && dnf clean all

# Copy the entire project into the container
COPY .. /fastlog

# Set the working directory to the fastlog project
WORKDIR /fastlog

# Build the fastlog library
RUN make

# Run the test program
RUN ./test

# Start a shell to interact with the container
CMD ["/bin/bash"]