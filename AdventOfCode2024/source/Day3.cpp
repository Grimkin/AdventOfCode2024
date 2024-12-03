#include <print>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <string>
#include <regex>
#include <sstream>
#include <variant>

namespace Day3
{
    struct Multiplication
    {
        int64_t left;
        int64_t right;
    };

    struct Do
    {};

    struct Dont
    {};

    using Instruction = std::variant<Multiplication, Do, Dont>;

    std::vector<Instruction> loadInput( std::istream& stream )
    {
        std::vector<Instruction> instructions;

        static std::regex regexMul( R"((mul)\((\d*),(\d*)\)|(do\(\))|(don't\(\)))" );

        std::stringstream buffer;
        buffer << stream.rdbuf();
        auto line = buffer.str();

        auto mul_begin = std::sregex_iterator( line.begin(), line.end(), regexMul );
        auto mul_end = std::sregex_iterator();

        for( std::sregex_iterator i = mul_begin; i != mul_end; ++i )
        {
            std::smatch match = *i;
            if( match[ 1 ].matched )
                instructions.push_back( Multiplication{ std::stoll( match[ 2 ] ), std::stoll( match[ 3 ] ) } );
            else if( match[ 4 ].matched )
                instructions.push_back( Do{} );
            else if( match[ 5 ].matched )
                instructions.push_back( Dont{} );
        }

        return instructions;
    }

    int64_t getValue( const Instruction& instruction )
    {
        if( auto multiplication = std::get_if<Multiplication>( &instruction ))
            return multiplication->left * multiplication->right;
        return 0;
    }

    int64_t getSumOfInstructions( const auto& instructions )
    {
        return std::ranges::fold_left(
            instructions | std::views::transform( getValue ),
            0ll, std::plus{} );
    }

    template<typename ... Callable>
    struct visitor : Callable... {
        using Callable::operator()...;
    };

    int64_t getSumOfFilteredInstructions( const std::vector<Instruction>& instructions )
    {
        int64_t sum = 0;
        bool active = true;

        for( auto& instruction : instructions )
        {
            std::visit( visitor{
                [ & ]( const Multiplication& multiplication ) { sum += active ? multiplication.left * multiplication.right : 0; },
                [ & ]( const Do& ) { active = true; },
                [ & ]( const Dont& ) { active = false; }
            }, instruction );
        }

        return sum;
    }
}

void executeDay3()
{
    std::ifstream file( "input/Day3.txt" );
    auto instructions = Day3::loadInput( file );
    std::println( "Day3: Sum of instructions: {}", Day3::getSumOfInstructions( instructions ) );
    std::println( "Day3: Sum of filtered instructions: {}", Day3::getSumOfFilteredInstructions( instructions ) );
    return;
}
