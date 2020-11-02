#include <vector>
#include <exception>
#include <iostream>
#include <sstream>

#include "IntComputer.hpp"

void test_instruction_parsing()
{
}

std::ostream &operator<<(std::ostream &out, const IntComputer::container_type values)
{
    std::string delim = "";
    out << "{";
    for (auto it = values.begin(); it != values.end(); it++)
    {
        out << delim << *it;
        if (delim == "")
            delim = ",";
    }
    out << "}";
    return out;
}

void test_programs()
{
    struct TestCase
    {
        IntComputer::container_type input;
        IntComputer::container_type output;
    };

    std::vector<TestCase> test_cases{
        {{99}, {99}},
        {{1, 0, 0, 0, 99}, {2, 0, 0, 0, 99}},
        {{2, 3, 0, 3, 99}, {2, 3, 0, 6, 99}},
        {{2, 4, 4, 5, 99, 0}, {2, 4, 4, 5, 99, 9801}},
        {{1, 1, 1, 4, 99, 5, 6, 0, 99}, {30, 1, 1, 4, 2, 5, 6, 0, 99}},
        {{1, 1, 1, 4, 99, 5, 6, 0, 99}, {30, 1, 1, 4, 2, 5, 6, 0, 99}}};

    for (auto test_case : test_cases)
    {
        try
        {
            IntComputer computer(test_case.input);
            computer.execute();
            if (computer.get_memory() == test_case.output)
                std::cout << "Passed.\n";
            else
                std::cout << "Failed: Got " << computer.get_memory() << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "Failed.\n";
            std::cerr << e.what() << '\n';
        }
    }
}

void test_io()
{
    struct io_test_case
    {
        IntComputer::container_type program;
        std::string input;
        std::string output;
    };

    std::vector<io_test_case> test_cases{
        {{3, 0, 4, 0, 99}, "66", "66\n"},
        {{104, 1125899906842624, 99}, "", "1125899906842624\n"},
        {{1102, 34915192, 34915192, 7, 4, 7, 99, 0}, "", "1219070632396864\n"},
    };

    for (auto test_case : test_cases)
    {
        std::istringstream in(test_case.input);
        std::ostringstream out;
        IntComputer computer(test_case.program, out, in);
        computer.execute();
        if (out.str() == test_case.output)
            std::cout << "Passed.\n";
        else
        {
            std::cout << "Failed. Expected `" << test_case.output << "` got `" << out.str() << "`\n";
        }
    }
}

void test_relatives()
{
    struct TestCase
    {
        IntComputer::container_type program;
        std::string input;
        std::string output;
    };

    std::vector<TestCase> test_cases{
        {{109, -1, 4, 1, 99}, "", "-1\n"},
        {{109, -1, 104, 1, 99}, "", "1\n"},
        {{109, -1, 204, 1, 99}, "", "109\n"},
        {{109, 1, 9, 2, 204, -6, 99}, "", "204\n"},
        {{109, 1, 109, 9, 204, -6, 99}, "", "204\n"},
        {{109, 1, 209, -1, 204, -106, 99}, "", "204\n"},
        {{109, 1, 3, 3, 204, 2, 99}, "1", "1\n"},
        {{109, 1, 203, 2, 204, 2, 99}, "1", "1\n"},
    };

    for (auto test_case : test_cases)
    {
        std::istringstream in(test_case.input);
        std::ostringstream out;
        IntComputer computer(test_case.program, out, in);
        computer.execute();
        if (out.str() == test_case.output)
            std::cout << "Passed.\n";
        else
        {
            std::cout << "Failed. Expected `" << test_case.output << "` got `" << out.str() << "`\n";
        }
    }
}

int main()
{
    std::cout << "Test basic functionality\n";
    test_programs();
    std::cout << "Test basic i/o\n";
    test_io();
    std::cout << "Test relatives\n";
    test_relatives();
    return 0;
}
