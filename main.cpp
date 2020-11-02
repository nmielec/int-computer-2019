#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <exception>
#include <algorithm>
#include <fstream>

#include "IntComputer.hpp"

const std::string usage = R"(
IntComputer : simple machine code interpreter
Usage :
    main.exe program
        program : comma separated program memory
    main.exe -f path
        path : path to file with one line of comma separated program memory

Arguments :
    --help, -h shows this help
    -f input program file
    --dump show dump before and after execution
    --debug, -d run in debug mode
    --input input string used inplace of cin

Debug mode :
    (s)top
    (n)ext
    (d)ump [start] [end]
    (r)un all
    (m)emory [address1] [address2] ...
    (i)nstruction pointer address

)";

class ArgParser
{
    typedef char const **ForwardIt;

    ForwardIt begin;
    ForwardIt end;

public:
    ArgParser(ForwardIt begin, ForwardIt end) : begin(begin),
                                                end(end)
    {
    }

    bool has_arg(std::string arg)
    {
        auto it = std::find(begin, end, arg);
        return (it != end) && ((it + 1) != end);
    }

    bool has_param(std::string arg)
    {
        auto it = std::find(begin, end, arg);
        return it != end;
    }

    std::string get_arg(std::string arg)
    {
        auto it = std::find(begin, end, arg);
        if (it != end && (it + 1) != end)
        {
            return *(it + 1);
        }
        else
        {
            std::stringstream ss;
            ss << "Missing arg " << arg;
            throw std::runtime_error(ss.str());
        }
    }
};

main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << usage << std::endl;
    }
    else
    {
        ArgParser argparser(argv, argv + argc);

        std::string program;
        if (argc == 2)
        {
            program = argv[1];
        }
        else
        {
            if (argv[1][0] != '-')
                program = argv[1];
            else
            {
                if (!argparser.has_arg("-f"))
                {
                    std::cerr << "If program is not passed through first parameter (-f)ile parameter has to be provided\n";
                    return -1;
                }
                else
                {
                    std::string path = argparser.get_arg("-f");
                    std::ifstream file(path, std::ios_base::in);
                    if (file)
                    {
                        std::getline(file, program);
                    }
                    else
                    {
                        std::cerr << "Could not open input file " << path;
                        return -1;
                    }
                }
            }
        }

        bool dump = argparser.has_param("--dump");
        bool debug = argparser.has_param("--debug") || argparser.has_param("-d");

        std::stringstream input;
        bool use_cin = true;
        if (argparser.has_arg("--input"))
        {
            input = std::stringstream(argparser.get_arg("--input"));
            use_cin = false;
        }

        if (!program.empty())
        {
            try
            {
                
                IntComputer computer(program, std::cout, use_cin ? std::cin : input);
                if (dump)
                    computer.dump_memory();

                if (!debug)
                    computer.execute();
                else
                {
                    std::string debug_cmd;
                    bool stop_debug = false;
                    while (!computer.has_halted() && !stop_debug)
                    {
                        do
                        {
                            std::cout << "$";
                            std::getline(std::cin, debug_cmd);
                            if (debug_cmd[0] == 's')
                            {
                                stop_debug = true;
                                break;
                            }
                            else if (debug_cmd[0] == 'n')
                                break;
                            else if (debug_cmd[0] == 'r')
                                computer.execute();
                            else if (debug_cmd[0] == 'i')
                                std::cout << computer.tellg() << "\n";
                            else if (debug_cmd[0] == 'd')
                            {
                                std::stringstream ss(debug_cmd);
                                ss.ignore(100, ' ');
                                size_t begin, end;
                                ss >> begin >> end;
                                computer.dump_memory(begin, end);
                            }
                            else if (debug_cmd[0] == 'm')
                            {
                                std::stringstream ss(debug_cmd);
                                ss.ignore(100, ' ');
                                std::size_t pos;
                                IntComputer::container_type memory = computer.get_memory();
                                while(ss >> pos)
                                {
                                    std::cout << memory[pos] << " ";
                                }
                                std::cout << "\n";
                            }
                            
                        } while (!debug_cmd.empty());
                        computer.step();
                    }
                }

                if (dump)
                    computer.dump_memory();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
    return 0;
}
