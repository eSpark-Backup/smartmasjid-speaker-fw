#pragma once
#include <ArduinoJson.h> // Make sure to include the ArduinoJson library
#include <unordered_map>
#include <cstring>

class SM_Json {
public:
  template <typename... Args>
  static void create(char* json, uint8_t size, Args... args) {
    // Create a JSON document
    StaticJsonDocument<256> doc;

    // Add values to the document using a fold expression
    (addValueToDoc(doc, args.first, args.second), ...);

    // Serialize the JSON document
    serializeJson(doc, json, size);
  }

private:
  template <typename T>
  static void addValueToDoc(JsonDocument& doc, const char* key, const T& value) {
    doc[key] = value;
  }
};
