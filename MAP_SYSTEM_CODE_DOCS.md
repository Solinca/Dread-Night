# Map System Code Documentation

## Overview

The map system provides functionality to display a global map showing the player's position and discovered locations (POIs and gatherable resources).

## C++ Components

### 1. UMapDiscoverySubsystem

**Location**: `Source/DreadNight/Public/MapSystem/MapDiscoverySubsystem.h`

A world subsystem that tracks discovered map locations.

**Key Functions**:
- `DiscoverLocation(WorldLocation, MarkerType, MarkerName)` - Manually register a discovered location
- `DiscoverActor(Actor)` - Discover an actor that implements IMapDiscoverable interface
- `GetDiscoveredMarkers()` - Get all discovered markers
- `ClearDiscoveredMarkers()` - Clear all discoveries (useful for new game)

**Enums**:
- `EMapMarkerType`: POI, Gatherable, Player

**Structs**:
- `FMapMarkerData`: Contains WorldLocation, MarkerType, MarkerName, bIsDiscovered

### 2. UMapWidget

**Location**: `Source/DreadNight/Public/UI/Widgets/MapWidget.h`

Base UUserWidget class for the map UI.

**Key Functions**:
- `RefreshMap()` - Updates the map display
- `GetDiscoveredMarkers()` - Retrieves all discovered markers
- `GetPlayerLocation()` - Gets the current player location

**Blueprint Events**:
- `OnMapRefreshed(Markers, PlayerLocation)` - Override this in Blueprint to update the UI

### 3. ADiscoverableMapLocation

**Location**: `Source/DreadNight/Public/MapSystem/DiscoverableMapLocation.h`

An actor that can be placed in the world to mark discoverable locations.

**Properties**:
- `LocationType` - Type of location (POI/Gatherable)
- `LocationName` - Display name
- `DiscoveryRadius` - How close the player must be to discover
- `bDiscoverOnBeginPlay` - Auto-discover on spawn

**Features**:
- Automatically discovers itself when player enters radius
- Disables tick after discovery for performance

### 4. IMapDiscoverable

**Location**: `Source/DreadNight/Public/MapSystem/MapDiscoverable.h`

Interface for actors that can be discovered on the map.

**Interface Methods**:
- `GetMarkerType()` - Returns the marker type
- `GetLocationName()` - Returns the display name
- `GetDiscoveryLocation()` - Returns the world location to mark

**Usage**: Implement this interface on any actor that should be discoverable.

### 5. CustomPlayerController Extensions

**Location**: `Source/DreadNight/Public/Player/CustomPlayerController.h`

**New Properties**:
- `MapWidgetClass` - Blueprint class for the map widget (set in editor)

**New Functions**:
- `DisplayMap()` - Toggles map display on/off

**Behavior**:
- Opening map: Creates widget, shows mouse cursor, switches to UI input mode
- Closing map: Hides widget, hides mouse cursor, returns to game input mode

## Usage Examples

### Discovering a Location from Blueprint

```cpp
// In any Blueprint actor
UMapDiscoverySubsystem* MapSubsystem = GetWorld()->GetSubsystem<UMapDiscoverySubsystem>();
MapSubsystem->DiscoverLocation(GetActorLocation(), EMapMarkerType::POI, "Ancient Ruins");
```

### Implementing IMapDiscoverable

```cpp
// In your gatherable resource actor header
UCLASS()
class AGatherableResource : public AActor, public IMapDiscoverable
{
    GENERATED_BODY()
    
public:
    virtual EMapMarkerType GetMarkerType_Implementation() const override
    {
        return EMapMarkerType::Gatherable;
    }
    
    virtual FString GetLocationName_Implementation() const override
    {
        return ResourceName;
    }
    
    virtual FVector GetDiscoveryLocation_Implementation() const override
    {
        return GetActorLocation();
    }
    
private:
    UPROPERTY(EditAnywhere)
    FString ResourceName;
};
```

### Using ADiscoverableMapLocation

1. Place `ADiscoverableMapLocation` actor in the level
2. Set properties in the editor:
   - Location Type: POI or Gatherable
   - Location Name: "Iron Mine", "Berry Bush", etc.
   - Discovery Radius: 1000.0 (units)
3. The location will auto-discover when the player gets near

### Calling Discovery from Other Systems

```cpp
// When player gathers a resource
void AGatherableActor::OnGathered()
{
    if (UMapDiscoverySubsystem* MapSubsystem = GetWorld()->GetSubsystem<UMapDiscoverySubsystem>())
    {
        MapSubsystem->DiscoverActor(this); // If this implements IMapDiscoverable
    }
}
```

## Integration with Existing Systems

### Interactable System

Discoverable locations can also be interactable:

```cpp
UCLASS()
class ADiscoverablePOI : public AActor, public IInteractable, public IMapDiscoverable
{
    // Implements both interfaces
};
```

### Save System

To persist discoveries across save/load:

1. Store the DiscoveredMarkers array in your save game
2. On load, iterate through saved markers and call DiscoverLocation for each
3. Consider adding serialization support to FMapMarkerData

## Performance Considerations

- `ADiscoverableMapLocation` disables tick after discovery
- Discovery uses a 100-unit radius check to prevent duplicates
- Consider using a spatial hash if you have hundreds of discoverable locations
- The map widget only refreshes when opened or when RefreshMap() is called

## Future Enhancements

Possible extensions to the system:

1. **Fog of War**: Only show explored areas of the map
2. **Categories**: Filter markers by type
3. **Fast Travel**: Click markers to teleport
4. **Quest Markers**: Add dynamic quest markers
5. **Player Drawings**: Allow players to mark custom locations
6. **Mini-map**: Use the same subsystem for a mini-map widget
7. **Multiplayer**: Sync discoveries across clients
8. **Save/Load**: Persist discoveries between sessions
