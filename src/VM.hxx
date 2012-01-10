/* Maciej Poleski (C) 2012 */

#ifndef VM_HXX
#define VM_HXX

#include <stdint.h>

#include <memory>
#include <string>

namespace VM
{
typedef uint16_t word;

class VM
{
    VM(const VM &) = delete;
    VM &operator=(const VM &) = delete;
public:
    VM();

    void loadMemory(const std::string &fileName);

    word run();

private:
    /// Rozmiar pamięci i przestrzeń adresowa
    static constexpr uint32_t m_memorySize = 0x10000;

    /// Pamięć
    std::shared_ptr<word> p_memory;

    //
    // Rejestry
    //

    /// Rejestr ogólnego przeznaczenia
    word m_registers[16];

    /// Rejestr stanu
    /// +---------------------------+--+--+--+--+--+
    /// |   nie zaimplementowane    |ZE|OF|SF|ZF|CF|
    /// +---------------------------+--+--+--+--+--+
    ///                               |  |  |  |  +-> Carry flag
    ///                               |  |  |  +----> Zero flag
    ///                               |  |  +-------> Sign flag
    ///                               |  +----------> Overflow flag
    ///                               +-------------> Division by zero exception
    /// Carry flag:
    /// Przeniesienie (liczby bez znaku)
    ///    - Wynik dodawania nie mieści się w rejestrze
    ///    - Wynik odejmowania jest mniejszy od zera
    /// Set if an arithmetic operation generates a carry or
    /// a borrow out of the most-significant bit of the result;
    /// cleared otherwise. This flag indicates an overflow condition for
    /// unsigned-integer arithmetic.
    ///
    /// Zero flag:
    /// Wynikiem ostatniej operacji jest 0
    ///
    /// Sign flag:
    /// Najstarszy bit wyniku
    ///
    /// Overflow flag:
    /// Przepełnienie (liczby ze znakiem)
    ///    - Wynik dodawania nie mieści się w rejestrze
    ///    - Wynik odejmowania nie mieści się w rejestrze
    /// Set if the integer result is too large a positive
    /// number or too small a negative number (excluding the sign-bit)
    /// to fit in the destination operand; cleared otherwise. This flag
    /// indicates an overflow condition for signed-integer (two’s
    /// complement) arithmetic.
    ///
    /// Zero exception:
    /// Próba dzielenia przez 0
    word m_state;

    /// Rejestr - wskaźnik na następną instrukcje do wykonania
    word m_instructionPtr;

    /// Rejestr - dedykowany do przechowywania adresu.
    word m_pointer;

    // TODO:
    // Porty we/wy
    // Być może przerwania/wyjątki
    // Uzupełnić luki funkcjonalności
};

}

#endif // VM_HXX
