# Linux 
### BSON LIBS:

Fetch from repo:

Prerequisites

```bash
sudo apt-get install cmake libssl-dev libsasl2-dev
```

```bash
apt-get install libbson-1.0
```

Build from Source:

```bash
sudo apt-get install libmongoc-1.0-0
sudo apt-get install libbson-1.0
sudo apt-get install cmake libssl-dev libsasl2-dev

wget https://github.com/mongodb/mongo-c-driver/releases/download/1.16.2/mongo-c-driver-1.16.2.tar.gz
tar xzf mongo-c-driver-1.16.2.tar.gz
cd mongo-c-driver-1.16.2
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
sudo make install
```

### Boost LIBS:
Fetch from repo:

```bash
sudo apt-get install libboost-all-dev
```

### SQLITE3

```bash
sudo apt-get install sqlite3 libsqlite3-dev
```

Build Libs from source:

```bash
wget http://www.sqlite.org/sqlite-autoconf-3070603.tar.gz
tar xvfz sqlite-autoconf-3070603.tar.gz
cd sqlite-autoconf-3070603
./configure
make
make install
```

### QT5 LIBS
```bash
sudo apt-get install qt5-default
```

### ALSA LIBS

```bash
sudo apt-get install libasound-dev
```

### Portaudio LIBS

```bash
sudo apt-get install libasound-dev 
git clone https://github.com/PortAudio/portaudio.git
cd portaudio
./configure
sudo make 
sudo make install 
sudo ldconfig
```

### OPUS LIBS

```
git clone https://gitlab.xiph.org/xiph/opus.git
cd opus
./autogen.sh
./configure
sudo make 
sudo make install
sudo ldconfig
```
