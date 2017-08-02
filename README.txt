# Set envvar

export PROJ_ROOT=$(pwd)

# Build gtest static library

apt-get intall libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/

