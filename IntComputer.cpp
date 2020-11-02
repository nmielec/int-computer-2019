#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <iomanip>

#include "IntComputer.hpp"

IntComputer::container_type IntComputer::parse_str(const std::string &str)
{
    std::stringstream stream(str);
    container_type values;
    value_type val;
    while (stream >> val)
    {
        values.push_back(val);
        if (stream.peek() == ',')
            stream.ignore();
    }

    if (stream.bad())
    {
        throw std::runtime_error("I/O Error while reading program");
    }
    else if (!stream.eof() && stream.fail())
    {
        stream.clear();

        char bad_char;
        stream >> bad_char;

        std::stringstream err;
        err << "Could not read character " << bad_char << " at position " << stream.tellg() << "\n";

        throw std::runtime_error(err.str());
    }
    return values;
}

IntComputer::value_type IntComputer::fetch()
{
    if (instruction_pointer >= memory.size())
        throw std::runtime_error("Out of bounds error");
    return (*this)[instruction_pointer++];
}

IntComputer::Instruction IntComputer::fetch_instruction()
{
    Instruction inst(fetch());
    return inst;
}

IntComputer::value_type IntComputer::fetch_value(IntComputer::OPMode mode)
{
    if (mode == OPMode::IMMEDIATE)
        return fetch();
    else if (mode == OPMode::POSITION)
        return (*this)[fetch()];
    else if (mode == OPMode::RELATIVE)
        return (*this)[fetch() + relative_base];
    else
    {
        std::stringstream ss("Unknown OP Mode ");
        ss << mode;
        throw std::runtime_error(ss.str());
    }
}

IntComputer::address_type IntComputer::fetch_address(IntComputer::OPMode mode)
{
    if (mode == OPMode::POSITION)
        return fetch();
    else if (mode == OPMode::RELATIVE)
        return fetch() + relative_base;
    else
    {
        throw std::runtime_error("Incompatible mode with address fetching.");
    }
}

void IntComputer::step()
{
    running = true;
    Instruction inst = fetch_instruction();
    switch (inst.code)
    {
    case OPCode::ADD:
    {
        value_type first = fetch_value(inst.modes[0]);
        value_type second = fetch_value(inst.modes[1]);
        address_type address = fetch_address(inst.modes[2]);
        store(address, first + second);
        break;
    }
    case OPCode::MUL:
    {
        value_type first = fetch_value(inst.modes[0]);
        value_type second = fetch_value(inst.modes[1]);
        address_type address = fetch_address(inst.modes[2]);
        store(address, first * second);
        break;
    }
    case OPCode::IN:
    {
        address_type address = fetch_address(inst.modes[0]);
        value_type val;
        if (&istream == &std::cin)
            ostream << ">";
        istream >> val;
        store(address, val);
        break;
    }
    case OPCode::OUT:
    {
        value_type value = fetch_value(inst.modes[0]);
        ostream << value << "\n";
        break;
    }
    case OPCode::JNZ:
    {
        value_type first = fetch_value(inst.modes[0]);
        value_type second = fetch_value(inst.modes[1]);
        if (first != 0)
            instruction_pointer = second;
        break;
    }
    case OPCode::JZ:
    {
        value_type first = fetch_value(inst.modes[0]);
        value_type second = fetch_value(inst.modes[1]);
        if (first == 0)
            instruction_pointer = second;
        break;
    }
    case OPCode::LT:
    {
        value_type first = fetch_value(inst.modes[0]);
        value_type second = fetch_value(inst.modes[1]);
        address_type address = fetch_address(inst.modes[2]);
        store(address, (first < second) ? 1 : 0);
        break;
    }
    case OPCode::EQ:
    {
        value_type first = fetch_value(inst.modes[0]);
        value_type second = fetch_value(inst.modes[1]);
        address_type address = fetch_address(inst.modes[2]);
        store(address, (first == second) ? 1 : 0);
        break;
    }
    case OPCode::SETB:
    {
        value_type base = fetch_value(inst.modes[0]);
        relative_base += base;
        break;
    }
    case OPCode::HALT:
        halted = true;
        break;
    default:
        std::stringstream ss;
        ss << "Unknown OP code " << inst.code << " at position " << instruction_pointer;
        throw std::runtime_error(ss.str());
        break;
    }
}

void IntComputer::execute()
{
    if (!memory.empty())
    {
        size_t i = 0;
        while (!halted && i++ < IntComputer::MAX_NUM_ITERATIONS)
            step();

        if(i == IntComputer::MAX_NUM_ITERATIONS+1)
            throw std::runtime_error("Reached max number of iterations");
    }
}

// template<typename T>
void print_formated_memory(std::vector<long long>::const_iterator begin, std::vector<long long>::const_iterator end, size_t column_w = 6, size_t columns = 6)
{
    std::cout << "MEMORY DUMP \n";
    std::cout << std::setw(column_w + 1) << " ";
    size_t size = end - begin;
    columns = columns > size ? size : columns;
    for (size_t i = 0; i < columns; i++)
    {
        std::cout << std::setw(column_w + 1) << i;
    }
    std::cout << "\n";
    for (auto it = begin; it != end; ++it)
    {
        auto offset = it - begin;
        if (offset % columns == 0)
        {
            if (offset)
                std::cout << "\n";

            std::cout << std::setfill('0') << std::setw(column_w) << offset << std::setfill(' ') << ": ";
        }
        std::cout << std::setw(column_w) << *it << " ";
    }
    std::cout << "\n";
}

void IntComputer::dump_memory() const
{
    dump_memory(0, memory.size());
}

void IntComputer::dump_memory(size_t begin, size_t end) const
{
    constexpr size_t columns = 8;
    constexpr size_t column_w = 10;

    container_type::const_iterator begin_it = (begin > memory.size()) ? memory.end() : memory.begin() + begin;
    container_type::const_iterator end_it = (end > memory.size()) ? memory.end() : memory.begin() + end;

    print_formated_memory(begin_it, end_it, column_w, columns);
}