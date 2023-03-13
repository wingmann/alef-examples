#include <alef/serializing.h>

#include <iostream>

int main(int argc, char** argv) {
    alf::ser::julianah obj{};
    obj["text"] = "Hello, Alef!";

    std::cout << obj.dump();
    return 0;
}
