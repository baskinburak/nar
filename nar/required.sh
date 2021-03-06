wget http://user.ceng.metu.edu.tr/~e1942697/nar.zip;
sudo apt-get update;
sudo apt-get -y install libjsoncpp-dev libjsoncpp0;
sudo apt-get -y install libcrypto++-dev;
sudo add-apt-repository ppa:ubuntu-toolchain-r/test;
sudo apt-get update;
sudo apt-get -y install gcc-5 g++-5;
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60 --slave /usr/bin/g++ g++ /usr/bin/g++-5;
sudo apt-get -y install make;
sudo apt-get -y install libpng-dev;
sudo apt-get -y install zlib1g-dev;
unzip nar.zip;
cd nar/narnode;
sudo date --set "20 Jan 2019 01:00";
make clean;
make;
mkdir ~/.config;
mkdir ~/.config/nar;
echo "alias nar='~/nar/narnode/narcli'" >> ~/.bashrc;
source ~/.bashrc;
