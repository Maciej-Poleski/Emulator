/* Maciej Poleski (C) 2012 */

#include "VM.hxx"

#include <fstream>
#include <stdexcept>

using namespace std;

VM::VM::VM() : m_running(false)
{
}

void VM::VM::loadMemory(const string &fileName)
{
    ifstream in(fileName, ios_base::in | ios_base::binary);
    in.read(reinterpret_cast<char *>(m_memory.data()), m_memorySize * sizeof(word));
    if(in.fail())
        throw runtime_error(u8"Nie udało się odczytać obrazu pamięci.");
    return;
}

VM::word VM::VM::run()
{
    throw logic_error(u8"Nie zaimplementowano procesora!");
}

VM::word &VM::VM::getRegister(Register what)
{
    if(static_cast<byte>(what) < 16)
        return m_registers[static_cast<byte>(what)];
    else if(what == Register::rip)
        return m_instructionPtr;
    else if(what == Register::rp)
        return m_pointer;
    else
        throw invalid_argument(u8"Wskazany rejestr nie istnieje");
}

void VM::VM::cpu_setCarryFlag()
{
    m_state |= 1;
}

void VM::VM::cpu_clearCarryFlag()
{
    m_state &= ~(1U);
}

void VM::VM::cpu_setZeroFlag()
{
    m_state |= 2;
}

void VM::VM::cpu_clearZeroFlag()
{
    m_state &= ~(2U);
}

void VM::VM::setSignFlag()
{
    m_state |= 4;
}

void VM::VM::clearSignFlag()
{
    m_state &= ~(4U);
}

void VM::VM::cpu_setOverflowFlag()
{
    m_state |= 8;
}

void VM::VM::cpu_clearOverflowFlag()
{
    m_state &= ~(8U);
}

void VM::VM::cpu_setDivisionByZeroFlag()
{
    m_state |= 16;
}

void VM::VM::cpu_clearDivisionByZeroFlag()
{
    m_state &= ~(16U);
}

VM::byte VM::VM::carryFlag() const
{
    return m_state & 1;
}

VM::byte VM::VM::zeroFlag() const
{
    return (m_state & 2) >> 1;
}

VM::byte VM::VM::signFlag() const
{
    return (m_state & 4) >> 2;
}

VM::byte VM::VM::overflowFlag() const
{
    return (m_state & 8) >> 3;
}

VM::byte VM::VM::divisionByZeroFlag() const
{
    return (m_state & 16) >> 4;
}

void VM::VM::setFlagsForResult(result what)
{
    if(what == 0)
        cpu_setZeroFlag();
    else
        cpu_clearZeroFlag();
    if(what & (1 << 14))
        cpu_setCarryFlag();
    else
        cpu_clearCarryFlag();
    if(what & (1 << 15))
        setSignFlag();
    else
        clearSignFlag();
    if(what & (1 << 16))
        cpu_setOverflowFlag();
    else
        cpu_clearOverflowFlag();
    cpu_clearDivisionByZeroFlag();
}

void VM::VM::cpu_add(Register destination, Register source)
{
    result t = getRegister(destination);
    t += getRegister(source);
    setFlagsForResult(t);
    getRegister(destination) = t;
}

void VM::VM::cpu_add(Register destination, word value)
{
    result t = getRegister(destination);
    t += value;
    setFlagsForResult(t);
    getRegister(destination) = t;
}

void VM::VM::cpu_add(Register source)
{
    result t = getRegister(source);
    t += getRegister(Register::r0);
    setFlagsForResult(t);
    getRegister(Register::r0) = t;
}

void VM::VM::cpu_addWithCarry(Register destination, Register source)
{
    result t = getRegister(destination);
    t += getRegister(source);
    t += carryFlag();
    setFlagsForResult(t);
    getRegister(destination) = t;
}

void VM::VM::cpu_addWithCarry(Register destination, word value)
{
    result t = getRegister(destination);
    t += value;
    t += carryFlag();
    setFlagsForResult(t);
    getRegister(destination) = t;
}

void VM::VM::cpu_and(Register destination, Register source)
{
    result t = getRegister(destination) & getRegister(source);
    setFlagsForResult(t);
    getRegister(destination) = t;
}

void VM::VM::cpu_and(Register destination, word value)
{
    result t = getRegister(destination) & value;
    setFlagsForResult(t);
    getRegister(destination) = t;
}

void VM::VM::cpu_and(Register source)
{
    result t = getRegister(Register::r0) & getRegister(source);
    setFlagsForResult(t);
    getRegister(Register::r0) = t;
}

void VM::VM::cpu_compareBySubstract(Register minuend, Register subtrahend)
{
    result t = getRegister(minuend);
    t -= getRegister(subtrahend);
    setFlagsForResult(t);
}

void VM::VM::cpu_compareBySubstract(Register minuend, word subtrahend)
{
    result t = getRegister(minuend);
    t -= subtrahend;
    setFlagsForResult(t);
}

void VM::VM::cpu_copyToFlagsFromRegister(Register source)
{
    m_state = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointer(Register source)
{
    m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointer(word value)
{
    m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfCarry(Register source)
{
    if(carryFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfCarry(word value)
{
    if(carryFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfDivisionByZero(Register source)
{
    if(divisionByZeroFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfDivisionByZero(word value)
{
    if(divisionByZeroFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfNotCarry(Register source)
{
    if(!carryFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfNotCarry(word value)
{
    if(!carryFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfNotDivisionByZero(Register source)
{
    if(!divisionByZeroFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfNotDivisionByZero(word value)
{
    if(!divisionByZeroFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfNotOverflow(Register source)
{
    if(!overflowFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfNotOverflow(word value)
{
    if(!overflowFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfNotSign(Register source)
{
    if(!signFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfNotSign(word value)
{
    if(!signFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfNotZero(Register source)
{
    if(!zeroFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfNotZero(word value)
{
    if(!zeroFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfOverflow(Register source)
{
    if(overflowFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfOverflow(word value)
{
    if(overflowFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfSign(Register source)
{
    if(signFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfSign(word value)
{
    if(signFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToInstructionPointerIfZero(Register source)
{
    if(zeroFlag())
        m_instructionPtr = getRegister(source);
}

void VM::VM::cpu_copyToInstructionPointerIfZero(word value)
{
    if(zeroFlag())
        m_instructionPtr = value;
}

void VM::VM::cpu_copyToMemoryFromRegister(address destination, Register source)
{
    m_memory[destination] = getRegister(source);
}

void VM::VM::cpu_copyToRegisterFromFlags(Register destination)
{
    getRegister(destination) = m_state;
}

void VM::VM::cpu_copyToRegisterFromMemory(Register destination, address source)
{
    getRegister(destination) = m_memory[source];
}

void VM::VM::cpu_copyToRegisterFromRegister(Register destination, Register source)
{
    getRegister(destination) = getRegister(source);
}

void VM::VM::cpu_decrement(Register destination)
{
    result t = getRegister(destination);
    --t;
    setFlagsForResult(t);
    getRegister(destination) = static_cast<word>(t);
}

void VM::VM::cpu_exchange(Register register1, Register register2)
{
    swap(getRegister(register1), getRegister(register2));
}

void VM::VM::cpu_haltVirtualMachine(word result)
{
    m_result = result;
    m_running = false;
}

void VM::VM::cpu_increment(Register destination)
{
    result t = getRegister(destination);
    ++t;
    setFlagsForResult(t);
    getRegister(destination) = static_cast<word>(t);
}

void VM::VM::cpu_loadToMemoryFromArgument(address destination, word value)
{
    m_memory[destination] = value;
}

void VM::VM::cpu_loadToRegisterFromArgument(Register destination, word value)
{
    getRegister(destination) = value;
}

void VM::VM::cpu_neg(Register destination)
{
    result t = getRegister(destination);
    t = -t;
    setFlagsForResult(t);
    getRegister(destination) = static_cast<word>(t);
}

void VM::VM::cpu_not(Register destination)
{
    result t = getRegister(destination);
    t = ~t;
    setFlagsForResult(t);
    getRegister(destination) = static_cast<word>(t);
}

void VM::VM::cpu_or(Register destination, Register source)
{
    result t = getRegister(destination);
    t |= getRegister(source);
    setFlagsForResult(t);
    getRegister(destination) = static_cast<word>(t);
}

void VM::VM::cpu_or(Register destination, word value)
{
    result t = getRegister(destination);
    t |= value;
    setFlagsForResult(t);
    getRegister(destination) = static_cast<word>(t);
}

void VM::VM::cpu_or(Register source)
{
    result t = getRegister(Register::r0);
    t |= getRegister(source);
    setFlagsForResult(t);
    getRegister(Register::r0) = static_cast<word>(t);
}

void VM::VM::cpu_pretendAnd(Register value1, Register value2)
{
    result t = getRegister(value1);
    t &= getRegister(value2);
    setFlagsForResult(t);
}

void VM::VM::cpu_pretendAnd(Register value1, word value2)
{
    result t = getRegister(value1);
    t &= value2;
    setFlagsForResult(t);
}

void VM::VM::cpu_signedDivide(Register destination, Register denominator)
{
    if(getRegister(denominator) == 0)
        cpu_setDivisionByZeroFlag();
    else {
        signed_result des = getRegister(destination);
        signed_result den = des;
        des /= getRegister(denominator);
        den %= getRegister(denominator);
        setFlagsForResult(des);
        getRegister(destination) = static_cast<word>(des);
        getRegister(denominator) = static_cast<word>(den);
    }
}

void VM::VM::cpu_signedMultiply(Register destination, Register source)
{
    signed_result r = getRegister(destination);
    r *= getRegister(source);
    setFlagsForResult(r);
    getRegister(destination) = static_cast<word>(r & (~static_cast<word>(0)));
    getRegister(source) = static_cast<word>(r >>(sizeof(word) * 2) & (~static_cast<word>(0)));
}

void VM::VM::cpu_signedRotateLeft(Register destination, byte rotate)
{

}
