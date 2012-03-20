/* Maciej Poleski (C) 2012 */

#ifndef VM_HXX
#define VM_HXX

#include <cstdint>

#include <array>
#include <string>

namespace VM
{
typedef std::uint16_t word;
typedef word address;
typedef std::uint8_t byte;
typedef std::uint32_t result;
typedef std::int32_t signed_result;

enum class Register : byte
{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    rip,
    rp,
};

class VM
{
    VM(const VM &) = delete;
    VM &operator=(const VM &) = delete;
public:
    VM();

    void loadMemory(const std::string &fileName);

    word run();

private:
    // Przenoszenie danych
    void cpu_copyToRegisterFromRegister(Register destination, Register source);
    void cpu_copyToRegisterFromMemory(Register destination, address source);
    void cpu_copyToMemoryFromRegister(address destination, Register source);
    void cpu_loadToRegisterFromArgument(Register destination, word value);
    void cpu_loadToMemoryFromArgument(address destination, word value); // ?
    void cpu_exchange(Register register1, Register register2);
    void cpu_copyToRegisterFromFlags(Register destination);
    void cpu_copyToFlagsFromRegister(Register source);
    // TODO: LEA (może)

    // Arytmetyka
    void cpu_add(Register destination, Register source);
    void cpu_add(Register destination, word value);
    void cpu_add(Register source);
    void cpu_substract(Register destination, Register source);
    void cpu_substract(Register destination, word value);
    void cpu_substract(Register source);
    void cpu_unsignedMultiply(Register destination, Register source);
    void cpu_signedMultiply(Register destination, Register source);
    void cpu_unsignedDivide(Register destination, Register denominator);
    void cpu_signedDivide(Register destination, Register denominator);
    void cpu_increment(Register destination);
    void cpu_decrement(Register destination);
    void cpu_neg(Register destination);
    void cpu_compareBySubstract(Register minuend, Register subtrahend);
    void cpu_compareBySubstract(Register minuend, word subtrahend);
    void cpu_addWithCarry(Register destination, Register source);
    void cpu_addWithCarry(Register destination, word value);
    void cpu_substractWithBorrow(Register destination, Register source);
    void cpu_substractWithBorrow(Register destination, word value);

    // Logika
    void cpu_and(Register destination, Register source);
    void cpu_and(Register destination, word value);
    void cpu_and(Register source);
    void cpu_or(Register destination, Register source);
    void cpu_or(Register destination, word value);
    void cpu_or(Register source);
    void cpu_xor(Register destination, Register source);
    void cpu_xor(Register destination, word value);
    void cpu_xor(Register source);
    void cpu_not(Register destination);
    void cpu_pretendAnd(Register value1, Register value2);
    void cpu_pretendAnd(Register value1, word value2);

    // Operacje bitowe
    void cpu_unsignedShiftLeft(Register destination, byte shift);
    void cpu_signedShiftLeft(Register destination, byte shift);
    void cpu_unsignedShiftRight(Register destination, byte shift);
    void cpu_signedShiftRight(Register destination, byte shift);
    void cpu_unsignedRotateLeft(Register destination, byte rotate);
    void cpu_signedRotateLeft(Register destination, byte rotate);
    void cpu_unsignedRotateRight(Register destination, byte rotate);
    void cpu_signedRotateRight(Register destination, byte rotate);
    void cpu_unsignedShiftLeftWithCarry(Register destination, byte shift);
    void cpu_signedShiftLeftWithCarry(Register destination, byte shift);
    void cpu_unsignedShiftRightWithCarry(Register destination, byte shift);
    void cpu_signedShiftRightWithCarry(Register destination, byte shift);
    void cpu_unsignedRotateLeftWithCarry(Register destination, byte rotate);
    void cpu_signedRotateLeftWithCarry(Register destination, byte rotate);
    void cpu_unsignedRotateRightWithCarry(Register destination, byte rotate);
    void cpu_signedRotateRightWithCarry(Register destination, byte rotate);

    // Skoki
    void cpu_copyToInstructionPointer(Register source);
    void cpu_copyToInstructionPointer(word value);
    void cpu_copyToInstructionPointerIfZero(Register source);
    void cpu_copyToInstructionPointerIfZero(word value);
    void cpu_copyToInstructionPointerIfNotZero(Register source);
    void cpu_copyToInstructionPointerIfNotZero(word value);
    void cpu_copyToInstructionPointerIfSign(Register source);
    void cpu_copyToInstructionPointerIfSign(word value);
    void cpu_copyToInstructionPointerIfNotSign(Register source);
    void cpu_copyToInstructionPointerIfNotSign(word value);
    void cpu_copyToInstructionPointerIfCarry(Register source);
    void cpu_copyToInstructionPointerIfCarry(word value);
    void cpu_copyToInstructionPointerIfNotCarry(Register source);
    void cpu_copyToInstructionPointerIfNotCarry(word value);
    void cpu_copyToInstructionPointerIfOverflow(Register source);
    void cpu_copyToInstructionPointerIfOverflow(word value);
    void cpu_copyToInstructionPointerIfNotOverflow(Register source);
    void cpu_copyToInstructionPointerIfNotOverflow(word value);
    void cpu_copyToInstructionPointerIfDivisionByZero(Register source);
    void cpu_copyToInstructionPointerIfDivisionByZero(word value);
    void cpu_copyToInstructionPointerIfNotDivisionByZero(Register source);
    void cpu_copyToInstructionPointerIfNotDivisionByZero(word value);

    // Obsługa stanu procesora
    void cpu_setCarryFlag();
    void cpu_clearCarryFlag();
    void cpu_setOverflowFlag();
    void cpu_clearOverflowFlag();
    void cpu_setZeroFlag();
    void cpu_clearZeroFlag();
    void cpu_setDivisionByZeroFlag();
    void cpu_clearDivisionByZeroFlag();
    void cpu_haltVirtualMachine(word result);

    // Pomocnicze
    word & getRegister(Register what);
    void setFlagsForResult(result what);
    void setSignFlag();
    void clearSignFlag();
    byte carryFlag() const;
    byte zeroFlag() const;
    byte signFlag() const;
    byte overflowFlag() const;
    byte divisionByZeroFlag() const;

    /// Rozmiar pamięci i przestrzeń adresowa
    static constexpr uint32_t m_memorySize = 0x10000;

    /// Pamięć
    std::array<word,m_memorySize> m_memory; // FIXME: Tak nie wolno obsługiwać tablic!

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

    /// Kod zwrócony z Maszyny Wirtualnej
    word m_result;

    /// Czy maszyna pracuje
    bool m_running;

    // TODO:
    // Porty we/wy
    // Być może przerwania/wyjątki
    // Uzupełnić luki funkcjonalności
};

}

#endif // VM_HXX
