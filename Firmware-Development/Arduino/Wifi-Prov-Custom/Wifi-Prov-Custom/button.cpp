#include "button.hpp"
#include <Arduino.h>
bool buttonPressed = false;
void handleButtonInterrupt();

void Button::setup()
{
  pinMode(buttonPin, INPUT);
  attachInterrupt(buttonPin, handleButtonInterrupt, RISING);
}
void Button::handleButtonInterrupt() 
{
  buttonPressed = true; // Assuming you have a global variable named buttonPressed
}

// void Button:: button_interrupt_handler() {
//     // This function gets executed whenever the button is pressed
//     // Send a request to the URL to get the status
//     httplib::Client cli("http://your_server");

//     auto res = cli.Get("/status");
//     if (res && res->status == 200) {
//         // Assuming the server returns a JSON response like: {"status": true}
//         // You may need to parse the JSON response accordingly.
//         bool status = res->body == "true";

//         if (status) {
//             std::cout << "Button is pressed." << std::endl;
//         } else {
//             std::cout << "Button is not pressed." << std::endl;
//         }
//     } else {
//         std::cout << "Failed to get the button status from the server." << std::endl;
//     }
// }