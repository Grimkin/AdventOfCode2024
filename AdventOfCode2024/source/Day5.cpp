#include <print>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <string>
#include <mdspan>
#include <map>
#include <list>
#include <set>

namespace Day5
{
    struct PageMap
    {
        std::multimap<int64_t, int64_t> ordering;
        std::vector<std::vector<int64_t>> updates;
    };

    PageMap loadInput( std::istream& stream )
    {

        std::multimap<int64_t, int64_t> ordering;
        std::vector<std::vector<int64_t>> updates;

        for( std::string line; std::getline( stream, line ); )
        {
            if( line.empty() )
                break;

            auto res = line
                | std::views::split( '|' )
                | std::views::transform( []( auto v ) { return std::stoll( std::string( v.begin(), v.end() ) ); } );

            ordering.insert( { *res.begin(), *std::next( res.begin() ) } );
        }

        for( std::string line; std::getline( stream, line ); )
        {
            updates.push_back( line
                | std::views::split( ',' )
                | std::views::transform( []( auto v ) { return std::stoll( std::string( v.begin(), v.end() ) ); } )
                | std::ranges::to<std::vector>() );
        }

        return { ordering, updates };
    }

    bool isUpdateValid( const std::vector<int64_t>& update, const std::multimap<int64_t, int64_t>& orderings )
    {
        for( auto [idx, page] : update | std::views::enumerate | std::views::drop( 1 ) )
        {
            auto [first, last] = orderings.equal_range( page );
            for( auto it = first; it != last; it++ )
            {
                auto end = update.begin() + idx + 1;
                if( std::find( update.begin(), end, it->second ) != end )
                    return false;
            }
        }
        return true;
    }

    int64_t getSumOfValidUpdates( const PageMap& pagemap )
    {
        auto isUpdateValid = [ & ]( auto& update ) { return Day5::isUpdateValid( update, pagemap.ordering ); };
        auto addMedian = []( auto sum, auto& update ) { return sum + update[ update.size() / 2 ]; };

        return std::ranges::fold_left(
            pagemap.updates | std::views::filter( isUpdateValid ),
            0ll,
            addMedian );
    }

    struct Order
    {
        std::set<int64_t> before;
        std::set<int64_t> after;
    };

    std::map<int64_t, Order> getOrderMap( const std::multimap<int64_t, int64_t>& orderings )
    {
        std::map<int64_t, Order> orderMap;
        for( auto& [left, right] : orderings )
        {
            orderMap[ left ].after.insert( right );
            orderMap[ right ].before.insert( left );
        }
        return orderMap;
    }

    int64_t getUpdateValue( std::vector<int64_t> update, std::map<int64_t, Order>& orderMap )
    {
        std::list<int64_t> res;
        for( auto& page : update )
        {
            auto found = std::ranges::find_first_of( res | std::views::reverse, orderMap[page].before );
            res.insert( found.base(), page );
        }
        return *std::next( res.begin(), res.size() / 2 );
    }

    bool isInVec( const std::vector<int64_t>& vec, int64_t val )
    {
        for( size_t j = 0; j < vec.size(); j++ )
        {
            if( vec[ j ] == val )
                return true;
        }
        return false;
    }
    int64_t getSumOfUpdatedInvalidUpdates( const PageMap& pagemap )
    {
        auto orderMap = getOrderMap( pagemap.ordering );

        auto isUpdateInvalid = [ & ]( auto& update ) { return !Day5::isUpdateValid( update, pagemap.ordering ); };

        return std::ranges::fold_left( pagemap.updates | std::views::filter( isUpdateInvalid ), 0ll, [ & ]( auto sum, auto& update ) { return sum + getUpdateValue( update, orderMap ); } );
    }
}

void executeDay5()
{
    std::ifstream file( "input/Day5.txt" );
    auto pageMap = Day5::loadInput( file );
    std::println( "Day5: Sum of valid updates: {}", Day5::getSumOfValidUpdates( pageMap ) );
    std::println( "Day5: Sum of invalid updates: {}", Day5::getSumOfUpdatedInvalidUpdates( pageMap ) );

    return;
}
