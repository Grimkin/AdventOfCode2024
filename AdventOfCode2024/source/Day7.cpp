#include <print>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <string>
#include <set>

namespace Day7
{
    struct Equation
    {
        int64_t result = 0;
        std::vector<int64_t> values;
    };

    enum class Operator
    {
        Plus,
        Times,
        Concat
    };

    std::vector<Equation> loadInput( std::istream& stream )
    {
        std::vector<Equation> equations;
        for( std::string line; std::getline( stream, line ); )
        {
            auto toInt = []( auto c ) { return std::stoll( std::string( c.begin(), c.end() ) ); };
            auto temp = line
                | std::views::filter( []( auto c ) { return c != ':'; } )
                | std::views::split( ' ' )
                | std::views::transform( toInt )
                | std::ranges::to<std::vector>();

            equations.push_back( { temp[ 0 ], { temp.begin() + 1, temp.end() } } );
        }
        return equations;
    }

    bool checkEquation( const Equation& equation, const std::vector<Operator>& operators )
    {
        int64_t result = equation.values[ 0 ];

        for( size_t i = 0; i < operators.size(); i++ )
        {
            switch( operators[i] )
            {
                case Operator::Plus:
                    result += equation.values[ i + 1 ];
                    break;
                case Operator::Times:
                    result *= equation.values[ i + 1 ];
                    break;
            }
        }

        return result == equation.result;
    }

    bool getNexOperator( std::vector<Operator>& operators )
    {
        for( auto& op : operators )
        {
            if( op == Operator::Plus )
            {
                op = Operator::Times;
                return true;
            }
            else
                op = Operator::Plus;
        }
        return false;
    }

    bool isEquationValid( const Equation& equation )
    {
        std::vector<Operator> operators( equation.values.size() - 1 );

        do
        {
            if( checkEquation( equation, operators ) )
                return true;
        } while( getNexOperator( operators ) );

        return false;
    }

    int64_t getSumOfValidEquations( const std::vector<Equation>& equations )
    {
        return std::ranges::fold_left( equations | std::views::filter( isEquationValid ) | std::views::transform( []( auto& e ) { return e.result; } ), 0ll, std::plus{} );
    }

    int64_t concat( int64_t lhs, int64_t rhs )
    {
        return std::stoll( std::to_string( lhs ) + std::to_string( rhs ) );
    }

    bool checkEquation2( const Equation& equation, const std::vector<Operator>& operators )
    {
        int64_t result = equation.values[ 0 ];

        for( size_t i = 0; i < operators.size(); i++ )
        {
            switch( operators[ i ] )
            {
                case Operator::Plus:
                    result += equation.values[ i + 1 ];
                    break;
                case Operator::Times:
                    result *= equation.values[ i + 1 ];
                    break;
                case Operator::Concat:
                    result = concat( result, equation.values[ i + 1 ] );
                    break;
            }
        }

        return result == equation.result;
    }

    bool getNexOperator2( std::vector<Operator>& operators )
    {
        for( auto& op : operators )
        {
            switch( op )
            {
                case Operator::Plus:
                    op = Operator::Times;
                    return true;
                case Operator::Times:
                    op = Operator::Concat;
                    return true;
                case Operator::Concat:
                    op = Operator::Plus;
            }
        }
        return false;
    }

    bool isEquationValid2( const Equation& equation )
    {
        std::vector<Operator> operators( equation.values.size() - 1 );

        do
        {
            if( checkEquation2( equation, operators ) )
                return true;
        } while( getNexOperator2( operators ) );

        return false;
    }
    int64_t getSumOfValidEquations2( const std::vector<Equation>& equations )
    {
        return std::ranges::fold_left( equations | std::views::filter( isEquationValid2 ) | std::views::transform( []( auto& e ) { return e.result; } ), 0ll, std::plus{} );
    }
}

void executeDay7()
{
    std::ifstream file( "input/Day7.txt" );
    auto equations = Day7::loadInput( file );
    std::println( "Day 7: Sum of valid equations: {}", Day7::getSumOfValidEquations( equations ) );
    std::println( "Day 7: Sum of valid equations with concat: {}", Day7::getSumOfValidEquations2( equations ) );
    return;
}
