FROM gitpod/workspace-full-vnc
                    
USER gitpod

# Install custom tools, runtime, etc. using apt-get
# For example, the command below would install "bastet" - a command line tetris clone:
#
# RUN sudo apt-get -q update && #     sudo apt-get install -yq bastet && #     sudo rm -rf /var/lib/apt/lists/*
#
# More information: https://www.gitpod.io/docs/config-docker/

RUN sudo apt-get update && \
    sudo apt-get install -y qt5-default qtcreator qt*5*

RUN sudo mv /usr/bin/start-vnc-session.sh /usr/bin/start-vnc-session.sh.bak && \
    sudo sed 's/1920/1024/g' /usr/bin/start-vnc-session.sh.bak  |sudo sed 's/1080/768/g' > /tmp/start-vnc-session.sh && \
    sudo mv /tmp/start-vnc-session.sh /usr/bin/start-vnc-session.sh && \
    sudo chmod +x /usr/bin/start-vnc-session.sh
