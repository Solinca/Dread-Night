# Map System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         USER INPUT                               │
│                    Press 'M' Key → IA_Map                        │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│              CustomPlayerController                              │
│  ┌───────────────────────────────────────────────────────┐      │
│  │ DisplayMap(InputActionValue)                          │      │
│  │  - Creates/Shows MapWidget                            │      │
│  │  - Toggles Input Mode (Game ↔ UI)                    │      │
│  │  - Shows/Hides Mouse Cursor                          │      │
│  └───────────────────────────────────────────────────────┘      │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│                      UMapWidget (C++ Base)                       │
│  ┌───────────────────────────────────────────────────────┐      │
│  │ RefreshMap()                                          │      │
│  │  - Gets discovered markers from subsystem             │      │
│  │  - Gets current player location                       │      │
│  │  - Calls OnMapRefreshed (Blueprint Event)            │      │
│  └───────────────────────────────────────────────────────┘      │
│                         │                                        │
│                         ▼                                        │
│  ┌───────────────────────────────────────────────────────┐      │
│  │ OnMapRefreshed (Blueprint Implementable Event)        │      │
│  │  - Input: Array<FMapMarkerData>, PlayerLocation       │      │
│  │  - Blueprint implements visual display logic          │      │
│  └───────────────────────────────────────────────────────┘      │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────────┐
│              UMapDiscoverySubsystem (World Subsystem)            │
│  ┌───────────────────────────────────────────────────────┐      │
│  │ Storage: TArray<FMapMarkerData> DiscoveredMarkers     │      │
│  ├───────────────────────────────────────────────────────┤      │
│  │ DiscoverLocation(Location, Type, Name)                │      │
│  │ DiscoverActor(Actor*)                                 │      │
│  │ GetDiscoveredMarkers() → Array                        │      │
│  │ ClearDiscoveredMarkers()                              │      │
│  └───────────────────────────────────────────────────────┘      │
└─────────────────────────▲──────────────────────────────────────┘
                          │
                          │ Register Discoveries
                          │
         ┌────────────────┴────────────────┐
         │                                  │
         ▼                                  ▼
┌──────────────────────┐        ┌──────────────────────┐
│ ADiscoverableMap     │        │  Any Actor           │
│ Location             │        │  (implements         │
├──────────────────────┤        │  IMapDiscoverable)   │
│ - LocationType       │        ├──────────────────────┤
│ - LocationName       │        │ GetMarkerType()      │
│ - DiscoveryRadius    │        │ GetLocationName()    │
│ - Auto-discovers     │        │ GetDiscoveryLocation │
│   on player approach │        │                      │
└──────────────────────┘        └──────────────────────┘
```

## Data Flow

### Discovery Flow:
```
1. Player approaches discoverable location
   ↓
2. ADiscoverableMapLocation::Tick() detects player
   ↓
3. Calls MapDiscoverySubsystem->DiscoverLocation()
   ↓
4. Subsystem adds to DiscoveredMarkers array
   ↓
5. Location disables tick (performance)
```

### Display Flow:
```
1. User presses 'M'
   ↓
2. CustomPlayerController::DisplayMap()
   ↓
3. Creates/Shows UMapWidget
   ↓
4. UMapWidget::RefreshMap()
   ↓
5. Gets markers from MapDiscoverySubsystem
   ↓
6. Calls OnMapRefreshed (Blueprint Event)
   ↓
7. Blueprint creates visual markers on UI
```

## Component Relationships

```
┌─────────────────────────────────────────────────┐
│         PRESENTATION LAYER (Blueprint)          │
│  - WBP_MapWidget                                │
│  - WBP_MapMarker                                │
│  - Visual design & marker placement             │
└─────────────────────┬───────────────────────────┘
                      │ Blueprint Implements
                      │ OnMapRefreshed Event
┌─────────────────────▼───────────────────────────┐
│         LOGIC LAYER (C++)                       │
│  - UMapWidget (base class)                      │
│  - CustomPlayerController (input handling)      │
│  - Coordinate conversion helpers                │
└─────────────────────┬───────────────────────────┘
                      │ Uses
                      │
┌─────────────────────▼───────────────────────────┐
│         DATA LAYER (C++)                        │
│  - UMapDiscoverySubsystem (storage)             │
│  - FMapMarkerData (struct)                      │
│  - EMapMarkerType (enum)                        │
└─────────────────────▲───────────────────────────┘
                      │ Populates
                      │
┌─────────────────────┴───────────────────────────┐
│         WORLD ACTORS (C++ & Blueprint)          │
│  - ADiscoverableMapLocation                     │
│  - IMapDiscoverable implementers                │
│  - Gatherable resources                         │
│  - POI actors                                   │
└─────────────────────────────────────────────────┘
```

## Extension Points

### 1. Custom Marker Types
Add to `EMapMarkerType` enum:
```cpp
UENUM(BlueprintType)
enum class EMapMarkerType : uint8
{
    POI,
    Gatherable,
    Player,
    Quest,      // Add new type
    Vendor,     // Add new type
    // ...
};
```

### 2. Additional Marker Data
Extend `FMapMarkerData` struct:
```cpp
USTRUCT(BlueprintType)
struct FMapMarkerData
{
    // Existing fields...
    
    UPROPERTY(BlueprintReadWrite)
    FLinearColor MarkerColor;  // Add custom color
    
    UPROPERTY(BlueprintReadWrite)
    int32 Priority;  // Add priority for rendering order
};
```

### 3. Custom Discovery Logic
Override in derived class:
```cpp
UCLASS()
class ACustomDiscoverable : public ADiscoverableMapLocation
{
    virtual void Tick(float DeltaTime) override
    {
        // Custom discovery logic
        if (CustomConditionMet())
        {
            DiscoverLocation();
        }
    }
};
```

### 4. Map Filters
Add to UMapWidget:
```cpp
UPROPERTY(BlueprintReadWrite)
TSet<EMapMarkerType> VisibleMarkerTypes;

TArray<FMapMarkerData> GetFilteredMarkers()
{
    return DiscoveredMarkers.FilterByPredicate([this](const FMapMarkerData& Marker)
    {
        return VisibleMarkerTypes.Contains(Marker.MarkerType);
    });
}
```
