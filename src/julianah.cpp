#include "julianah.h"

#include "alef/serializing/julianah.h"

void hello_json() {
    auto obj = alf::ser::json_load(R"({
        "json literals are": {
            "strings": ["foo", "bar"],
            "numbers": [42, 6.62606975e-34],
            "boolean values": [true, false],
            "objects": {
                "null": null,
                "another": null
            }
        }
    })");

    std::cout << obj << "\n\n";
}
