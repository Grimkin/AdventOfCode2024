#include <print>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <string>
#include <mdspan>

namespace Day4
{
    struct Vec2
    {
        int64_t x = 0;
        int64_t y = 0;
    };
    struct WordMap
    {
        WordMap( std::vector<char> data, const Vec2 size )
            : data( std::move( data ) )
            , map( std::mdspan( this->data.data(), size.x, size.y ) )
        {}

    private:
        std::vector<char> data;
    public:
        std::mdspan<char, std::extents<int64_t, std::dynamic_extent, std::dynamic_extent>> map;
    };

    WordMap loadInput( std::istream& stream )
    {
        std::vector<char> data;

        Vec2 size;
        for( std::string line; std::getline( stream, line ); size.y++ )
        {
            data.insert( data.end(), line.begin(), line.end() );
            size.x = line.size();
        }

        return { data, size };
    }

    bool checkXmas( const auto& map, const Vec2& startPos, const std::array<Vec2,3>& offsets )
    {
        return
            map[ startPos.x + offsets[ 0 ].x, startPos.y + offsets[ 0 ].y ] == 'M' &&
            map[ startPos.x + offsets[ 1 ].x, startPos.y + offsets[ 1 ].y ] == 'A' &&
            map[ startPos.x + offsets[ 2 ].x, startPos.y + offsets[ 2 ].y ] == 'S';
    }

    int64_t getNumXmasesAt( const std::mdspan<char, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>>& map, const Vec2& pos )
    {
        if( map[ pos.x, pos.y ] != 'X' )
            return 0;

        int64_t numXmases = 0;
        if( pos.x + 3 < map.extent( 0 ) )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ 1, 0 }, Vec2{ 2, 0 }, Vec2{ 3, 0 } } );
        if( pos.x >= 3 )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ -1, 0 }, Vec2{ -2, 0 }, Vec2{ -3, 0 } } );
        if( pos.y + 3 < map.extent( 1 ) )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ 0, 1 }, Vec2{ 0, 2 }, Vec2{ 0, 3 } } );
        if( pos.y >= 3 )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ 0, -1 }, Vec2{ 0, -2 }, Vec2{ 0, -3 } } );

        if( pos.x + 3 < map.extent( 0 ) && pos.y + 3 < map.extent( 1 ) )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ 1, 1 }, Vec2{ 2, 2 }, Vec2{ 3, 3 } } );
        if( pos.x + 3 < map.extent( 0 ) && pos.y >= 3 )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ 1, -1 }, Vec2{ 2, -2 }, Vec2{ 3, -3 } } );
        if( pos.x >= 3 && pos.y + 3 < map.extent( 1 ) )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ -1, 1 }, Vec2{ -2, 2 }, Vec2{ -3, 3 } } );
        if( pos.x >= 3 && pos.y >= 3 )
            numXmases += checkXmas( map, pos, std::array{ Vec2{ -1, -1 }, Vec2{ -2, -2 }, Vec2{ -3, -3 } } );

        return numXmases;
    }

    int64_t getNumXMas( const WordMap& wordMap )
    {
        int64_t numXmases = 0;
        for( int64_t y = 0; y < wordMap.map.extent(1); y++ )
            for( int64_t x = 0; x < wordMap.map.extent(0); x++ )
                numXmases += getNumXmasesAt( wordMap.map, { x, y } );
        return numXmases;
    }

    bool isMasCross( const auto& map, const Vec2& startPos )
    {
        return map[ startPos.x, startPos.y ] == 'A' &&
           (( 
                map[ startPos.x - 1, startPos.y - 1 ] == 'M' &&
                map[ startPos.x + 1, startPos.y + 1 ] == 'S' &&
                map[ startPos.x - 1, startPos.y + 1 ] == 'M' &&
                map[ startPos.x + 1, startPos.y - 1 ] == 'S'
            ) ||
            ( 
                map[ startPos.x - 1, startPos.y - 1 ] == 'M' &&
                map[ startPos.x + 1, startPos.y + 1 ] == 'S' &&
                map[ startPos.x - 1, startPos.y + 1 ] == 'S' &&
                map[ startPos.x + 1, startPos.y - 1 ] == 'M'
            ) ||
            (
                map[ startPos.x - 1, startPos.y - 1 ] == 'S' &&
                map[ startPos.x + 1, startPos.y + 1 ] == 'M' &&
                map[ startPos.x - 1, startPos.y + 1 ] == 'M' &&
                map[ startPos.x + 1, startPos.y - 1 ] == 'S'
            ) ||
            (
                map[ startPos.x - 1, startPos.y - 1 ] == 'S' &&
                map[ startPos.x + 1, startPos.y + 1 ] == 'M' &&
                map[ startPos.x - 1, startPos.y + 1 ] == 'S' &&
                map[ startPos.x + 1, startPos.y - 1 ] == 'M'
            ));
    }

    int64_t getNumMasCrosses( const WordMap& wordMap )
    {
        int64_t numXmases = 0;
        for( int64_t y = 1; y < wordMap.map.extent( 1 ) - 1; y++ )
        {
            for( int64_t x = 1; x < wordMap.map.extent( 0 ) - 1; x++ )
            {
                numXmases += isMasCross( wordMap.map, { x, y } );
            }
        }

        return numXmases;
    }
}

void executeDay4()
{
    std::ifstream file( "input/Day4.txt" );
    auto wordMap = Day4::loadInput( file );
    std::println( "Day4: Number of xmas in map: {}", Day4::getNumXMas( wordMap ) );
    std::println( "Day4: Number of xmas in map: {}", Day4::getNumMasCrosses( wordMap ) );
}
