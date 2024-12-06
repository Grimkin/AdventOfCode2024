#include <print>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <string>
#include <set>

namespace Day6
{
    struct Vec2
    {
        int64_t x = 0;
        int64_t y = 0;
        auto operator<=>( const Vec2& ) const = default;
    };

    struct MapData
    {
        Vec2 startPosition;
        std::set<Vec2> obstacles;
        Vec2 size;
    };

    MapData loadInput( std::istream& stream )
    {
        MapData mapData;
        for( std::string line; std::getline( stream, line ); mapData.size.y++ )
        {
            mapData.size.x = line.size();
            for( int64_t i = 0; i < line.size(); i++ )
            {
                switch( line[ i ] )
                {
                    case '#':
                        mapData.obstacles.emplace( i, mapData.size.y );
                        break;
                    case '^':
                        mapData.startPosition = { i, mapData.size.y };
                        break;
                }
            }
        }
        return mapData;
    }

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    Direction turnRight( Direction direction )
    {
        switch( direction )
        {
            case Direction::Up:
                return Direction::Right;
            case Direction::Down:
                return Direction::Left;
            case Direction::Left:
                return Direction::Up;
            case Direction::Right:
                return Direction::Down;
        }
        throw std::runtime_error( "invalid direciton" );
    }

    Vec2 getOffset( Direction direction )
    {
        switch( direction )
        {
            case Direction::Up:
                return { 0, -1 };
            case Direction::Down:
                return { 0, 1 };
            case Direction::Left:
                return { -1, 0 };
            case Direction::Right:
                return { 1, 0 };
        }
        throw std::runtime_error( "invalid direciton" );
    }

    bool moveOrTurn( Vec2& position, Direction& direction, const std::set<Vec2>& obstacles, const Vec2& mapSize )
    {
        auto offset = getOffset( direction );

        if( obstacles.count( { position.x + offset.x, position.y + offset.y } ) )
            direction = turnRight( direction );
        else
            position = { position.x + offset.x, position.y +offset.y };

        return position.x >= 0 && position.y >= 0 && position.x < mapSize.x && position.y < mapSize.y;
    }

    auto getVisitedPositions( const MapData& mapData )
    {
        std::set<Vec2> visitedPositions{ mapData.startPosition };
        auto direction( Direction::Up );
        auto position( mapData.startPosition );
        while( moveOrTurn( position, direction, mapData.obstacles, mapData.size ) )
            visitedPositions.insert( position );
        return visitedPositions;
    }

    int64_t getNumVisitedPositions( const MapData& mapData )
    {
        return getVisitedPositions( mapData ).size();
    }

    bool isLoop( const MapData& mapData )
    {
        std::set<std::pair<Vec2, Direction>> visitedPositions{ { mapData.startPosition, Direction::Up } };
        auto direction( Direction::Up );
        auto position( mapData.startPosition );

        while( moveOrTurn( position, direction, mapData.obstacles, mapData.size ) )
        {
            if( visitedPositions.count( { position, direction } ) > 0 )
                return true;
            visitedPositions.insert( { position, direction } );
        }

        return false;
    }

    int64_t getNumPossibleLoops( const MapData& mapData )
    {
        auto pathPositions = getVisitedPositions( mapData );
        pathPositions.erase( mapData.startPosition );

        auto createsLoop = 
            [ & ]( const Vec2& position ) {
                MapData testMapData{ mapData };
                testMapData.obstacles.insert( position );
                return Day6::isLoop( testMapData );
            };

        return std::ranges::count_if( pathPositions, createsLoop );
    }
}

void executeDay6()
{
    std::ifstream file( "input/Day6.txt" );
    auto mapData = Day6::loadInput( file );
    std::println( "Day 6: Number of positions visited by the guard: {}", Day6::getNumVisitedPositions( mapData ) );
    std::println( "Day 6: Number of possible loops: {}", Day6::getNumPossibleLoops( mapData ) );
}
