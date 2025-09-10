~~~
g++ -std=c++17 -o zbar_qr_detect main.cpp zbar_detector.cpp \
     `pkg-config --cflags --libs opencv4` \
     -I/opt/homebrew/include \
     /opt/homebrew/lib/libzbar.dylib
~~~
