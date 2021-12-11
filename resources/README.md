#Linux 
### BSON LIBS:

Fetch from repo:

'''bash
apt-get install libbson-1.0
'''

Build from Source:

'''bash
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.12.0/mongo-c-driver-1.12.0.tar.gz
tar xzf mongo-c-driver-1.12.0.tar.gz
cd mongo-c-driver-1.12.0
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
'''

###Boost LIBS:
Fetch from repo:

'''bash
sudo apt-get install libboost-all-dev
'''

###SQLITE3

'''bash
sudo apt-get install sqlite3
'''

Build Libs from source:

'''bash
wget http://www.sqlite.org/sqlite-autoconf-3070603.tar.gz
tar xvfz sqlite-autoconf-3070603.tar.gz
cd sqlite-autoconf-3070603
./configure
make
make install
'''

###QT5 LIBS
'''bash
sudo apt-get install qt5-default
'''

###ALSA LIBS

'''bash
sudo apt-get install libasound-dev
'''

###Portaudio LIBS

'''bash
sudo apt-get install portaudio19-dev
'''

