#include <iostream>
#include <jsoncpp/json/json.h>
#include <sstream>

std::string json_schema = R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "$id": "http://example.com/product.schema.json",
    "title": "product",
    "description": "A product from Acme's catalog",
    "type": "object",
    "properties": {
        "productId": {
            "description": "The unique identifier for a product",
            "type": "integer"
        },
        "productName": {
            "description": "Name of the product",
            "type": "string"
        },
        "tags": {
            "description": "Tags for the product",
            "type": "array",
            "items": {
                "type": "string"
            }
        },
        "dimensions": {
            "type": "object",
            "properties": {
                "length": {
                    "type": "number"
                },
                "width": {
                    "type": "number"
                },
                "height": {
                    "type": "number"
                }
            },
            "required": ["length", "width", "height"]
        }
    },
    "required": ["productId", "productName"]
}
)";


inline Json::Value getJsonValueFromString(std::string const &in)
{
    Json::Value root;
    Json::CharReaderBuilder jsonReader;
    std::string errs;
    std::stringstream inStream(in);
    if (!Json::parseFromStream(jsonReader, inStream, &root, &errs))
    {
        throw std::runtime_error(errs);
    }
    return root;
}

struct object
{
    std::string property;
    std::vector<object> members;

    void print(std::string const& prefix = "") const
    {
        std::cout << prefix << property << std::endl;
        for (auto const &m: members)
        {
            m.print(prefix+"\t");
        }
    }
};

std::vector<object> getObject(Json::Value const &json_value_schema)
{
    std::vector<object> object_vec;
    for (auto const &property : json_value_schema["properties"].getMemberNames())
    {
        auto property_type_string = json_value_schema["properties"][property]["type"].asString();

        object o;
        o.property = property;
        if ("object" == property_type_string)
        {
            o.members = getObject(json_value_schema["properties"][property]);
        }
        object_vec.emplace_back(o);

    }
    return object_vec;
}

int main()
{
    auto json_value_schema = getJsonValueFromString(json_schema);
    auto object_vec = getObject(json_value_schema);
    for (auto const &o : object_vec)
    {
        o.print();
    }
    return 0;
}