# CarbonIntensity
api for carbonintensity.org.uk site provides access to environmental monitoring data, including key parameters such as humidity, temperature, and carbon dioxide levels, from selected areas within NOI Techpark Bolzano and Brunico
# main
```cpp
#include "CarbonIntensity.h"
#include <iostream>

int main() {
   CarbonIntensity api;
    auto wales = api.get_regional_wales().then([](json::value result) {
        std::cout << result<< std::endl;
    });
    wales.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
