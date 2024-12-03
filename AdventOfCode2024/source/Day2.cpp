#include <print>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <string>

namespace Day2
{
    struct Report
    {
        std::vector<int64_t> data;
    };

    std::vector<Report> loadInput( std::istream& stream )
    {
        std::vector<Report> result;

        for( std::string line; std::getline( stream, line ); )
        {
            result.push_back( Report{ line
                | std::views::split( ' ' )
                | std::views::transform( []( auto c ) { return std::stoll( std::string( c.begin(), c.end() ) ); } )
                | std::ranges::to<std::vector>()
                } );
        }
        return result;
    }

    bool isDataSafe( auto data )
    {
        auto neighbors = data | std::views::adjacent<2>;

        auto isInRange = []( auto v ) { auto dif = std::abs( std::get<0>( v ) - std::get<1>( v ) ); return dif >= 1 && dif <= 3; };

        return
            std::ranges::all_of( neighbors, isInRange ) &&
            ( std::ranges::all_of( neighbors, []( auto v ) { return std::get<0>( v ) < std::get<1>( v ); } ) ||
            std::ranges::all_of( neighbors, []( auto v ) { return std::get<0>( v ) > std::get<1>( v ); } ) );
    }

    bool isReportSafe( const Report& report )
    {
        return isDataSafe( report.data );
    }

    size_t getNumberOfSafeReports( const std::vector<Report>& reports )
    {
        return std::ranges::count_if( reports, isReportSafe );
    }

    bool isReportSafeWithRemove( Report report )
    {
        if( isDataSafe( report.data ) )
            return true;

        for( size_t i = 0; i < report.data.size(); i++ )
        {
            std::vector<int64_t> data;
            data.insert( data.end(), report.data.begin(), report.data.begin() + i );
            data.insert( data.end(), report.data.begin() + i + 1, report.data.end() );
            if( isDataSafe( data ) )
                return true;
        }
        return false;
    }

    size_t getNumberOfSafeReportsWithRemove( const std::vector<Report>& reports )
    {
        return std::ranges::count_if( reports, isReportSafeWithRemove );
    }
}

void executeDay2()
{
    std::ifstream file( "input/Day2.txt" );
    auto reports = Day2::loadInput( file );
    std::println( "Number of safe reports: {}", Day2::getNumberOfSafeReports( reports ) );
    std::println( "Number of safe reports: {}", Day2::getNumberOfSafeReportsWithRemove( reports ) );

    return;
}