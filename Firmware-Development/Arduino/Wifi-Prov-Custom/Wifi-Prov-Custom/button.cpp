class Button {
  public:
    void setup() 
    {
        pinMode(buttonPin, INPUT);
        attachInterrupt(buttonPin, handleButtonInterrupt, RISING);
    }
    void handleButtonInterrupt()
    {
        buttonPressed = true;
    }

  // private:
  //   void buttonPressed() 
  //   {
  //     Serial.println("Button pressed!");
  //   }
};