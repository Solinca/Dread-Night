# Map System Implementation Guide (GP-5)

This guide explains how to complete the map system implementation in Unreal Engine Editor.

## Overview

The C++ foundation for the map system has been implemented. This includes:
- **UMapDiscoverySubsystem**: Tracks discovered POIs and gatherable resources
- **UMapWidget**: Base C++ class for the map UI widget
- **CustomPlayerController::DisplayMap**: Input handler to toggle the map

## Steps to Complete in Unreal Editor

### 1. Create the Map Input Action

1. Navigate to `Content/Core/Inputs/Actions/`
2. Right-click → Input → Input Action
3. Name it `IA_Map`
4. Open the asset and set:
   - Value Type: Boolean

### 2. Add Map Action to Input Mapping Context

1. Navigate to `Content/Core/Inputs/IMC/`
2. Open the existing Input Mapping Context (likely `IMC_Default` or similar)
3. Add a new mapping:
   - Action: `IA_Map`
   - Key: `M` (keyboard)
   - Modifiers: None
   - Triggers: Pressed

### 3. Configure Player Controller Blueprint

1. Navigate to `Content/Core/Blueprints/Player/`
2. Open `BP_CustomPlayerController` (or the Blueprint derived from ACustomPlayerController)
3. In the Details panel, find **IA_Setup** array
4. Add a new element:
   - Action: Select `IA_Map`
   - Event: `Started`
   - Action Name: Select `DisplayMap` from the dropdown

5. In the **UI** category, set:
   - Map Widget Class: `WBP_MapWidget` (will be created in step 4)

### 4. Create the Map Widget Blueprint

1. Navigate to `Content/Core/Blueprints/UI/Widgets/`
2. Right-click → User Interface → Widget Blueprint
3. Name it `WBP_MapWidget`
4. Open the widget and set Parent Class to `MapWidget` (the C++ class)

5. Design the UI:

   **Canvas Panel** (Root)
   └─ **Image** (Background - semi-transparent black overlay)
      └─ **Canvas Panel** (Map Container)
          ├─ **Image** (Map Background/Terrain - fill this with your map texture)
          ├─ **Overlay** (Markers Container - for dynamic markers)
          └─ **Border** (Title/Header)
              └─ **Text Block** ("World Map")

6. Implement the **OnMapRefreshed** event:
   - Switch to Graph view
   - Override the `OnMapRefreshed` event (Blueprint Implementable Event from C++)
   - This event provides:
     - `Markers`: Array of FMapMarkerData
     - `PlayerLocation`: FVector

7. In the OnMapRefreshed event graph:
   - Clear all existing markers from the Overlay
   - Loop through the Markers array
   - For each marker, create a marker widget (see step 5)
   - Position markers based on WorldLocation converted to 2D map coordinates
   - Add a player position marker using the PlayerLocation

### 5. Create Marker Widget (Optional but Recommended)

1. Navigate to `Content/Core/Blueprints/UI/Widgets/`
2. Right-click → User Interface → Widget Blueprint
3. Name it `WBP_MapMarker`
4. Design:
   - **Overlay**
     └─ **Image** (Marker Icon - different icons for POI, Gatherable, Player)
        └─ **Text Block** (Marker Name)

5. Add exposed variables:
   - `MarkerType`: EMapMarkerType
   - `MarkerName`: String
   - `bIsPlayer`: Boolean

6. Create a function `SetMarkerData` that:
   - Takes FMapMarkerData as input
   - Updates the icon based on MarkerType
   - Sets the text to MarkerName
   - Changes color/style based on type

### 6. Add Close Button Functionality

1. In `WBP_MapWidget`, add a Close Button
2. Bind the button's OnClicked event to call `DisplayMap` again (to toggle closed)
3. Alternatively, pressing `M` again or `ESC` should close the map

### 7. Coordinate System Conversion

In the `OnMapRefreshed` event, you'll need to convert world 3D coordinates to 2D map coordinates:

```blueprint
Function: WorldToMapPosition
Input: WorldLocation (Vector)
Output: MapPosition (Vector2D)

// Get world bounds (you'll need to define these based on your map)
WorldMinX = -50000
WorldMaxX = 50000
WorldMinY = -50000
WorldMaxY = 50000

// Get map widget size
MapWidth = 1024  // or get from widget
MapHeight = 1024

// Convert
NormalizedX = (WorldLocation.X - WorldMinX) / (WorldMaxX - WorldMinX)
NormalizedY = (WorldLocation.Y - WorldMinY) / (WorldMaxY - WorldMinY)

MapX = NormalizedX * MapWidth
MapY = NormalizedY * MapHeight

Return (MapX, MapY)
```

### 8. Testing the Discovery System

To test discovering locations, you can:

1. Create a test actor blueprint (e.g., `BP_TestPOI`)
2. In its BeginPlay:
   ```blueprint
   Get World → Get Subsystem (MapDiscoverySubsystem)
   → Discover Location (Self Location, POI, "Test Location")
   ```

3. For gatherable resources, call `DiscoverLocation` when the player interacts with or gets near the resource

4. Example for player proximity detection:
   ```blueprint
   In your gatherable actor:
   - Add a Sphere Collision component
   - On Begin Overlap with Player:
     - Get World → Get Subsystem (MapDiscoverySubsystem)
     - Discover Location (Self Location, Gatherable, "Iron Ore")
   ```

### 9. Visual Design Recommendations

- Use a stylized/hand-drawn map texture for the background
- Player marker: Bright color (blue/green arrow)
- POI markers: Yellow/Gold icons (stars, buildings)
- Gatherable markers: Resource-specific icons (ore, plants, etc.)
- Add fog of war effect for undiscovered areas (optional)
- Add tooltips when hovering over markers

### 10. Additional Features (Optional)

- **Map Zoom**: Implement zoom in/out with mouse wheel
- **Map Pan**: Allow dragging to pan the map
- **Legend**: Add a legend showing what each marker type means
- **Quest Markers**: Extend the system to support quest markers
- **Fast Travel**: Click on discovered POIs to fast travel

## Testing

1. Compile the C++ code (if not already done)
2. Open the Unreal project
3. Create the assets as described above
4. Play in Editor (PIE)
5. Press `M` to open the map
6. Verify the player position is shown
7. Test discovery by approaching test POIs/gatherables
8. Press `M` again to close the map

## Troubleshooting

- **Map doesn't open**: Check that MapWidgetClass is set in BP_CustomPlayerController
- **No markers shown**: Ensure OnMapRefreshed is implemented and markers are being created
- **Input not working**: Verify IA_Map is added to the Input Mapping Context and bound correctly
- **Compile errors**: Ensure all C++ files are saved and project is regenerated

## Code Reference

The C++ code is located in:
- `Source/DreadNight/Public/MapSystem/MapDiscoverySubsystem.h`
- `Source/DreadNight/Private/MapSystem/MapDiscoverySubsystem.cpp`
- `Source/DreadNight/Public/UI/Widgets/MapWidget.h`
- `Source/DreadNight/Private/UI/Widgets/MapWidget.cpp`
- `Source/DreadNight/Public/Player/CustomPlayerController.h`
- `Source/DreadNight/Private/Player/CustomPlayerController.cpp`
