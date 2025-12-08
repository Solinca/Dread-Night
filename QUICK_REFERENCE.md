# Map System - Quick Reference

## What Was Implemented

✅ **C++ Foundation Complete** - All backend code is ready
- Map discovery tracking system
- Map widget base class
- Player controller integration
- Helper classes for discoverable locations
- Interface for flexible integration

## What You Need to Do in Unreal Editor

### Step 1: Create Input Action
📁 `Content/Core/Inputs/Actions/`
- Create new Input Action: `IA_Map`
- Set to Boolean type

### Step 2: Add to Input Mapping
📁 `Content/Core/Inputs/IMC/`
- Open existing Input Mapping Context
- Add mapping: `M` key → `IA_Map`

### Step 3: Configure Player Controller
📁 `Content/Core/Blueprints/Player/`
- Open `BP_CustomPlayerController`
- Add to **IA_Setup** array:
  - Action: `IA_Map`
  - Event: Started
  - Action Name: `DisplayMap`
- Set **Map Widget Class**: `WBP_MapWidget` (from Step 4)

### Step 4: Create Map Widget
📁 `Content/Core/Blueprints/UI/Widgets/`
- Create new Widget Blueprint: `WBP_MapWidget`
- Parent Class: `MapWidget`
- Design your UI (see implementation guide for details)
- Override `OnMapRefreshed` event to display markers

### Step 5: Test
1. Press Play
2. Press `M` to open map
3. Current player position should be visible
4. Place test discoverable locations to test discovery

## How to Make Locations Discoverable

### Option 1: Use ADiscoverableMapLocation Actor
```
Place in level → Set properties → Automatic discovery
```

### Option 2: Implement IMapDiscoverable Interface
```cpp
Your Actor → Implements → MapDiscoverable
Override: GetMarkerType, GetLocationName, GetDiscoveryLocation
```

### Option 3: Manual Discovery
```blueprint
Get World → Get Subsystem (MapDiscoverySubsystem)
→ Discover Location(Location, Type, Name)
```

## Key Bindings
- **M** - Toggle Map (configurable via IA_Map)
- **ESC** - Close Map (when in UI mode)
- **Mouse** - Navigate map UI

## Files Reference
- 📖 Detailed Guide: `MAP_IMPLEMENTATION_GUIDE.md`
- 📖 Code Docs: `MAP_SYSTEM_CODE_DOCS.md`
- 💻 C++ Code: `Source/DreadNight/Public|Private/MapSystem/`

## Common Issues

❌ **Map doesn't open**
→ Check MapWidgetClass is set in BP_CustomPlayerController

❌ **M key does nothing**
→ Verify IA_Map is in Input Mapping Context and bound to DisplayMap

❌ **No markers showing**
→ Implement OnMapRefreshed event in WBP_MapWidget blueprint

❌ **Compile errors**
→ Regenerate Visual Studio project files from .uproject

## Support
See `MAP_IMPLEMENTATION_GUIDE.md` for detailed instructions and troubleshooting.
