#!/bin/bash



sudo apt-get -y install g++
sudo apt-get -y install net-tools 
sudo apt-get -y install git
sudo apt-get -y install libpcap-dev libssl-dev cmake

git clone https://github.com/mfontanini/libtins.git
cd libtins/
mkdir build
cd build/
cmake ../
make
sudo make install
sudo ldconfig

cd ..
