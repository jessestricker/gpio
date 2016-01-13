#include "gpio.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#define BCM2835_NO_DELAY_COMPATIBILITY // prevent bcm2835.h from defining delay
#include <bcm2835.h>

// private definitions
namespace gpio {
struct setup_t {
    static bool s_initialized;

    setup_t();
    ~setup_t();
};

static setup_t s_setup;
} // namespace gpio

// struct setup_t
bool gpio::setup_t::s_initialized = false;

// init bcm
gpio::setup_t::setup_t()
{
    std::cout << "Initializing the BCM interface...";

    if (s_initialized)
        return;

    if (!bcm2835_init()) {
        std::cout << "fail" << std::endl;
        throw std::runtime_error("Could not initialize the BCM interface!");
    }

    std::cout << "success" << std::endl;
    s_initialized = true;
}

// close bcm
gpio::setup_t::~setup_t()
{
    std::cout << "Closing the BCM interface...";

    if (!bcm2835_close()) {
        std::cout << "fail" << std::endl;
        return;
    }

    std::cout << "success" << std::endl;
}

// class pin
std::vector<unsigned int> gpio::pin::s_registeredPins;

gpio::pin::pin(unsigned int number)
    : m_number(number)
{
    // check if registered
    for (auto& pin : s_registeredPins) {
        if (pin == number) {
            throw std::invalid_argument("pin already in use.");
        }
    }

    // register pin
    s_registeredPins.push_back(number);
}

gpio::pin::~pin()
{
    // unregister pin
    for (auto i = s_registeredPins.begin(); i != s_registeredPins.end(); ++i) {
        if (*i == m_number) {
            s_registeredPins.erase(i);
            break;
        }
    }
}

bool gpio::pin::state() const { return bcm2835_gpio_lev(number()) == HIGH; }

// class input_pin
gpio::input_pin::input_pin(unsigned int number)
    : pin(number)
{
    bcm2835_gpio_fsel(number, BCM2835_GPIO_FSEL_INPT);
}

// class output_pin
gpio::output_pin::output_pin(unsigned int number)
    : pin(number)
{
    bcm2835_gpio_fsel(number, BCM2835_GPIO_FSEL_OUTP);
    set_state(false); // reset state
}

void gpio::output_pin::set_state(bool value)
{
    bcm2835_gpio_write(number(), (value ? HIGH : LOW));
}

// class button_pin
gpio::button_pin::button_pin(unsigned int number)
    : input_pin(number)
{
    // set resistor to pull up
    bcm2835_gpio_set_pud(number, BCM2835_GPIO_PUD_UP);
}

bool gpio::button_pin::state() const
{
    // invert button state, because low is button pressed and high is button
    // released
    return !input_pin::state();
}

// non-gpio functions
void delay(unsigned long milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}