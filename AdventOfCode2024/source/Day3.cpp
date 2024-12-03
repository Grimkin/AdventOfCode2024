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
    struct Instruction
    {
        int64_t left;
        int64_t right;
        size_t position;
    };

    struct Data
    {
        std::vector<Instruction> instructions;
        std::vector<size_t> doPositions;
        std::vector<size_t> dontPositions;
    };

    std::vector<size_t> findAllLocations( const std::string& string, std::string_view value )
    {
        std::vector<size_t> positions;
        size_t offset = 0;
        while( offset < string.size() )
        {
            offset = string.find( value, offset );
            if( offset != std::string::npos )
                positions.push_back( offset++ );
        }
        return positions;
    }

    Data loadInput( std::istream& stream )
    {
        Data data;

        static std::regex regexMul( R"(mul\((\d*),(\d*)\))" );

        std::stringstream buffer;
        buffer << stream.rdbuf();
        auto line = buffer.str();

        auto mul_begin = std::sregex_iterator( line.begin(), line.end(), regexMul );
        auto mul_end = std::sregex_iterator();

        for( std::sregex_iterator i = mul_begin; i != mul_end; ++i )
        {
            std::smatch match = *i;
            data.instructions.push_back( { std::stoll( match[ 1 ] ), std::stoll( match[ 2 ] ), size_t( match.position() ) });
        }

        data.doPositions = findAllLocations( line, "do()" );
        data.dontPositions = findAllLocations( line, "don't()" );

        return data;
    }

    int64_t getSumOfInstructions( const auto& instructions )
    {
        return std::ranges::fold_left(
            instructions | std::views::transform( []( auto v ) { return v.left * v.right; } ),
            0ll, std::plus{} );
    }

    int64_t getSumOfFilteredInstructions( const Data& data )
    {
        int64_t sum = 0;
        size_t i = 0;
        size_t dontIdx = 0;
        size_t doIdx = 0;

        while( i < data.instructions.size() )
        {
            for(; data.instructions[ i ].position < data.dontPositions[ dontIdx ]; i++ )
                sum += data.instructions[ i ].left * data.instructions[ i ].right;

            while( data.doPositions[ doIdx ] < data.dontPositions[ dontIdx ] )
                if( ++doIdx >= data.doPositions.size() )
                    return sum;

            for( ; data.instructions[ i ].position < data.doPositions[ doIdx ]; i++ );

            while( data.doPositions[ doIdx ] > data.dontPositions[ dontIdx ] )
                if( ++dontIdx >= data.dontPositions.size() )
                    return sum + getSumOfInstructions( std::ranges::subrange( data.instructions.begin() + i, data.instructions.end() ) );
        }

        return sum;
    }
}

void executeDay3()
{
    std::ifstream file( "input/Day3.txt" );
    auto data = Day3::loadInput( file );
    std::println( "Day3: Sum of instructions: {}", Day3::getSumOfInstructions( data.instructions ) );
    std::println( "Day3: Sum of filtered instructions: {}", Day3::getSumOfFilteredInstructions( data ) );
    return;
}
