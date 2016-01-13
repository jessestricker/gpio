#ifndef GPIO_H
#define GPIO_H

#include <vector>

/// GPIO enthält Klassen zur einfachen Steuerung von den GPIO Pins des Raspberry Pi.
namespace gpio {
    
/// Die Basisklasse für einen Pin. Sie enthält Methoden zum Abfragen von Nummer und Status.
class pin {
public:
    virtual ~pin();
    
    /// Gibt die physikalische Pinnummer zurück, über die der Pin angesprochen wird.
    unsigned int number() const { return m_number; }
    
    /// Gibt den Pinstatus zurück.
    /// Ist der Pin auf HIGH, wird true zurückgegeben, bei LOW false.
    virtual bool state() const;

protected:
    explicit pin(unsigned int number);

private:
    static std::vector<unsigned int> s_registeredPins;
    unsigned int m_number;
};

/// Die Klasse input_pin liest einen digitalen Pin.
class input_pin : public pin {
public:
    explicit input_pin(unsigned int number);
};

/// Die Klasse output_pin setzt einen digitalen Pin.
class output_pin : public pin {
public:
    explicit output_pin(unsigned int number);

    /// Setzt den Pinstatus. Ist value true, wird der Pin auf HIGH gesetzt,
    /// andernfalls auf LOW.
    void set_state(bool value);
};

// class used for digital buttons connected to GND, derrived of input
class button_pin : public input_pin {
public:
    explicit button_pin(unsigned int number);

    /// Gibt den Pinstatus zurück.
    /// Ist der Knopf gedrückt, wird true zurückgegeben, andernfalls false.
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