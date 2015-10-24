#ifndef GPIO_H
#define GPIO_H

#include <chrono>
#include <cinttypes>
#include <vector>

// classes

namespace gpio {
// base class for all common pin functionality
class pin {
public:
    virtual ~pin();

    unsigned int number() const { return m_number; }
    virtual bool state() const;

protected:
    explicit pin(uint8_t number);

private:
    static std::vector<uint8_t> s_registeredPins;
    uint8_t m_number;
};

// class used for digital reading a certain pin
class input_pin : public pin {
public:
    explicit input_pin(uint8_t number);
    virtual ~input_pin() {}
};

// class used for digital writing to a certain pin
class output_pin : public pin {
public:
    explicit output_pin(uint8_t number);
    virtual ~output_pin() {}

    void set_state(bool value);
};

// class used for digital buttons connected to GND, derrived of input
class button_pin : public input_pin {
public:
    explicit button_pin(uint8_t number);
    virtual ~button_pin() {}

    virtual bool state() const override;
};
}

// functions
#ifdef __cplusplus
extern "C" {
#endif

void delay(unsigned long milliseconds);

#ifdef __cplusplus
}
#endif

#endif // GPIO_H