#include <print>
#include <vector>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <functional>
#include <set>

namespace Day1
{
    struct LocationLists
    {
        std::vector<int64_t> left;
        std::vector<int64_t> right;
    };

    LocationLists loadInput( std::istream& stream )
    {
        LocationLists result;
        int64_t a, b;
        while( !stream.eof() )
        {
            stream >> a;
            stream >> b;
            result.left.push_back( a );
            result.right.push_back( b );
        }
        return result;
    }

    int64_t getSumOfSmallestDistance( LocationLists lists)
    {
        std::ranges::sort( lists.left );
        std::ranges::sort( lists.right );

        auto dif = []( auto a, auto b ) { return std::abs( a - b ); };

        return std::ranges::fold_left( std::views::zip_transform( dif, lists.left, lists.right ), 0ll, std::plus{} );
    }

    int64_t getSimilarityValue( const LocationLists& lists )
    {
        std::multiset<int64_t> compareList{ lists.right.begin(), lists.right.end() };

        return std::ranges::fold_left( lists.left, 0ll, 
            [&]( auto sum, auto val ) {
                return sum + val * compareList.count( val );
            } );
    }
}

void executeDay1()
{
    std::ifstream input( "Input/Day1.txt" );
    auto lists = Day1::loadInput( input );
    std::println( "Day 1: Sum of location distances: {}", getSumOfSmallestDistance( lists ) );
    std::println( "Day 1: Similarity value: {}", getSimilarityValue( lists ) );
}
