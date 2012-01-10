/* Maciej Poleski (C) 2012 */

#include "VM.hxx"

#include <fstream>
#include <stdexcept>

using namespace std;

VM::VM::VM() : p_memory(new word[m_memorySize])
{
}

void VM::VM::loadMemory(const std::string &fileName)
{
    ifstream in(fileName,ios_base::in | ios_base::binary);
    in.read(reinterpret_cast<char*>(p_memory.get()),m_memorySize);
    if(in.fail())
        throw runtime_error(u8"Nie udało się odczytać obrazu pamięci.");
    return;
}

VM::word VM::VM::run()
{
    throw logic_error(u8"Nie zaimplementowano procesora!");
}
