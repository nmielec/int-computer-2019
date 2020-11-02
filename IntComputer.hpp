#ifndef INTCOMPUTER_HPP
#define INTCOMUTER_HPP

#include <vector>
#include <string>
#include <memory>
#include <sstream>

class IntComputer
{
public:
    const size_t MAX_NUM_ITERATIONS = 1000000;

    typedef long long value_type;
    typedef std::vector<value_type> container_type;
    typedef value_type address_type;
    typedef value_type op_code;

    enum OPCode
    {
        NOP = 0,
        ADD = 1,
        MUL = 2,
        IN = 3,
        OUT = 4,
        JNZ = 5,
        JZ = 6,
        LT = 7,
        EQ = 8,
        SETB = 9,

        HALT = 99,
    };
    enum OPMode
    {
        POSITION = 0,
        IMMEDIATE = 1,
        RELATIVE = 2,
    };

    struct Instruction
    {
        value_type code;
        OPMode modes[3];

        Instruction(value_type value)
        {
            code = value % 100;
            modes[0] = OPMode((value / 100) % 10);
            modes[1] = OPMode((value / 1000) % 10);
            modes[2] = OPMode((value / 10000) % 10);
        }
    };

private:
    container_type memory;
    size_t instruction_pointer = 0;
    value_type relative_base = 0;

    bool use_cin = false;
    bool running = false;
    bool halted = false;
    std::stringstream in_stream;
    std::ostream &ostream;
    std::istream &istream;

    value_type fetch();
    Instruction fetch_instruction();
    value_type fetch_value(IntComputer::OPMode);
    address_type fetch_address(IntComputer::OPMode mode);
    void store(size_t pos, value_type val)
    {
        if (pos >= memory.size())
            memory.resize(pos + 1);
        memory[pos] = val;
    }
    value_type operator[](size_t pos)
    {
        if (pos >= memory.size())
            memory.resize(pos + 1);
        return memory[pos];
    }

    static container_type parse_str(const std::string &);

public:
    IntComputer(std::ostream &ostream = std::cout, std::istream &istream = std::cin) : memory(),
                                                                                       instruction_pointer(0),
                                                                                       use_cin(true),
                                                                                       running(false),
                                                                                       ostream(ostream),
                                                                                       istream(istream)
    {
    }
    IntComputer(const std::string &input, std::ostream &ostream = std::cout, std::istream &istream = std::cin) : memory(parse_str(input)),
                                                                                                                 instruction_pointer(0),
                                                                                                                 use_cin(true),
                                                                                                                 running(false),
                                                                                                                 ostream(ostream),
                                                                                                                 istream(istream)
    {
    }

    IntComputer(const container_type &memory, std::ostream &ostream = std::cout, std::istream &istream = std::cin) : memory(memory),
                                                                                                                     instruction_pointer(0),
                                                                                                                     use_cin(true),
                                                                                                                     running(false),
                                                                                                                     ostream(ostream),
                                                                                                                     istream(istream)
    {
    }

    size_t tellg() const { return instruction_pointer; }
    void step();
    void execute();
    void dump_memory() const;
    void dump_memory(size_t begin, size_t end) const;
    bool has_halted() const { return halted; }
    const container_type &get_memory() const { return memory; }
};

#endif