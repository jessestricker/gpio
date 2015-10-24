#include <iostream> // std::cout, std::endl
#include <gpio.h> // gpio::*

void print_pin(gpio::pin& pin)
{
    std::cout << "pin " << pin.number()
              << " state = " << (pin.state() ? "HIGH" : "LOW") << "\n";
}

int main()
{
    // input class
    std::cout << "=== GPIO::INPUT_PIN ===\n";
    auto inputPin = gpio::input_pin(0);
    print_pin(inputPin);

    // ouput class
    std::cout << "=== GPIO::OUPUT_PIN ===\n";
    auto outputPin = gpio::output_pin(1);
    print_pin(outputPin);

    outputPin.set_state(true);
    print_pin(outputPin);

    outputPin.set_state(false);
    print_pin(outputPin);

    // button class
    std::cout << "=== GPIO::BUTTON_PIN ===\n";
    auto buttonPin = gpio::button_pin(2);
    print_pin(buttonPin);

    // delay
    std::cout << "=== DELAY ===\n";
    delay(300);

    return EXIT_SUCCESS;
}