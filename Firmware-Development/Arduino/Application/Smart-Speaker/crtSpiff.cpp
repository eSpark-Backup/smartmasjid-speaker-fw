
#include"crtSpiff.hpp"

// Method to initialize SPIFFS
void SPIFFSHandler::setUp() 
{
  if (!SPIFFS.begin(true)) 
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
  } 
  else 
  {
    Serial.println("SPIFFS mounted successfully");
  }
}

// Method to check if the file exists
void SPIFFSHandler::check() 
{
  if (SPIFFS.exists(FILE_TITLE)) 
  {
    Serial.println("File exists");
  } 
  else 
  {
    Serial.println("File does not exist");
  }
}

// Method to write new content to the file
void SPIFFSHandler::write(const char* content) 
{
  File file = SPIFFS.open("/test_example.txt", FILE_WRITE);
  if (!file) 
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.print(content);
  file.close();
  Serial.println("File written successfully");
}

// Method to read and print the content of the file
void SPIFFSHandler::print() 
{
  File file = SPIFFS.open("/test_example.txt", FILE_READ);
  if (!file) 
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  char crt[50];  // Adjust the size as needed for your specific content
  int i = 0;
  while (file.available() && i < sizeof(crt) - 1) 
  {
    crt[i] = file.read();
    i++;
  }
  crt[i] = '\0';  // Null-terminate the string

  Serial.println("File Content:");
  Serial.println(crt);
  file.close();
}

char* SPIFFSHandler::get() {
  File file = SPIFFS.open(FILE_TITLE, FILE_READ);
  if (!file) 
  {
    Serial.println("Failed to open file for reading");
    return nullptr;
  }

  size_t size = file.size();  // Get the size of the file
  if (size == 0) 
  {
    Serial.println("File is empty");
    file.close();
    return nullptr;
  }

  char* content = new char[size + 1];  // Allocate memory for the content (+1 for null terminator)
  file.readBytes(content, size);
  content[size] = '\0';  // Null-terminate the string

  file.close();
  return content;
}
