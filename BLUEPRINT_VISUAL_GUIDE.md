# Blueprint Setup Visual Guide

This guide provides a visual walkthrough of the Blueprint setup steps required to complete the map implementation.

---

## Step 1: Create Input Action (IA_Map)

**Location**: `Content/Core/Inputs/Actions/`

1. Right-click in Content Browser
2. Select: `Input` → `Input Action`
3. Name: `IA_Map`
4. Double-click to open
5. Set **Value Type**: `Boolean`

```
┌─────────────────────────────────────┐
│ Input Action: IA_Map                │
├─────────────────────────────────────┤
│ Value Type:     [Boolean]      ▼   │
│ Consume Input:  ☑                   │
│ Trigger When Paused: ☐              │
└─────────────────────────────────────┘
```

---

## Step 2: Add to Input Mapping Context

**Location**: `Content/Core/Inputs/IMC/`

1. Open existing Input Mapping Context (e.g., `IMC_Default`)
2. Click `+` to add new mapping
3. Set **Action**: `IA_Map`
4. Set **Key**: `M` (Keyboard)

```
┌──────────────────────────────────────────────────┐
│ Input Mapping Context                            │
├──────────────────────────────────────────────────┤
│ Mappings:                                        │
│  ┌────────────────────────────────────────────┐ │
│  │ Action: IA_Move                            │ │
│  │ Key: W, A, S, D                           │ │
│  └────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────┐ │
│  │ Action: IA_Look                            │ │
│  │ Key: Mouse XY                             │ │
│  └────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────┐ │
│  │ ⭐ Action: IA_Map                    [NEW] │ │
│  │    Key: M                                  │ │
│  │    Modifiers: None                         │ │
│  │    Triggers: Pressed                       │ │
│  └────────────────────────────────────────────┘ │
│  [+] Add Mapping                               │
└──────────────────────────────────────────────────┘
```

---

## Step 3: Configure Player Controller Blueprint

**Location**: `Content/Core/Blueprints/Player/BP_CustomPlayerController`

### 3A: Add Input Action to IA_Setup Array

1. Open `BP_CustomPlayerController`
2. Select the Blueprint (root)
3. In Details panel, find **Inputs** section
4. Expand **IA_Setup** array
5. Click `+` to add new element
6. Configure the new element:

```
┌─────────────────────────────────────────────────┐
│ Details: BP_CustomPlayerController              │
├─────────────────────────────────────────────────┤
│ Inputs                                          │
│  ├─ Mapping Context: IMC_Default                │
│  └─ IA_Setup                                    │
│     ├─ [0] Move                                 │
│     ├─ [1] Look                                 │
│     ├─ [2] Jump                                 │
│     ├─ [3] Sprint                               │
│     ├─ ...                                      │
│     └─ [N] ⭐ NEW ELEMENT                       │
│         ├─ Action: IA_Map              [Select] │
│         ├─ Event: Started              [Select] │
│         └─ Action Name: DisplayMap     [Select] │
│                                                  │
│  [+] Add Element                                │
└─────────────────────────────────────────────────┘
```

**Important**: In **Action Name** dropdown, you'll see all available functions. Select `DisplayMap`.

### 3B: Set Map Widget Class

1. Still in Details panel
2. Scroll to **UI** section
3. Set **Map Widget Class**:

```
┌─────────────────────────────────────────────────┐
│ Details: BP_CustomPlayerController              │
├─────────────────────────────────────────────────┤
│ UI                                              │
│  └─ Map Widget Class: WBP_MapWidget   [Select] │
│                                                  │
│ Movement                                        │
│  ├─ Base Move Speed: 600.0                     │
│  └─ ...                                         │
└─────────────────────────────────────────────────┘
```

---

## Step 4: Create Map Widget Blueprint

**Location**: `Content/Core/Blueprints/UI/Widgets/`

### 4A: Create Widget Blueprint

1. Right-click in Content Browser
2. Select: `User Interface` → `Widget Blueprint`
3. Name: `WBP_MapWidget`
4. Open the widget

### 4B: Set Parent Class

1. Click **File** → **Reparent Blueprint**
2. Search for: `MapWidget`
3. Select: `MapWidget` (C++ class)
4. Click **Select**

```
┌─────────────────────────────────────────────────┐
│ Reparent Blueprint                              │
├─────────────────────────────────────────────────┤
│ Search: MapWidget                               │
│                                                  │
│ ├─ UserWidget                                   │
│ │  ├─ PlayerHud                                 │
│ │  ├─ SmartProgressBar                          │
│ │  └─ ⭐ MapWidget                    [SELECT]  │
│ │     └─ Your custom C++ widget base            │
│ └─ ...                                          │
│                                                  │
│         [Select]                [Cancel]        │
└─────────────────────────────────────────────────┘
```

### 4C: Design the UI (Designer Tab)

**Suggested Layout:**

```
┌─────────────────────────────────────────────────┐
│ WBP_MapWidget (Designer View)                   │
├─────────────────────────────────────────────────┤
│                                                  │
│  Canvas Panel (Root)                            │
│   │                                             │
│   ├─ Image (Background Overlay)                │
│   │   - Anchor: Fill Screen                    │
│   │   - Color: Black (0,0,0,0.8)               │
│   │                                             │
│   └─ Canvas Panel (Map Container)              │
│       - Anchor: Center                         │
│       - Size: 1024x1024                        │
│       │                                         │
│       ├─ Image (Map Background)                │
│       │   - Brush: Your map texture            │
│       │                                         │
│       ├─ Overlay (Markers Container)           │
│       │   - Will be populated in Blueprint     │
│       │                                         │
│       ├─ Border (Title Header)                 │
│       │   └─ Text: "World Map"                 │
│       │                                         │
│       └─ Button (Close Button)                 │
│           └─ Text: "X" or "Close"              │
│                                                  │
└─────────────────────────────────────────────────┘
```

**Widget Hierarchy (left panel):**
```
[CanvasPanel] (Root)
├─ [Image] BackgroundOverlay
└─ [CanvasPanel] MapContainer
   ├─ [Image] MapBackground
   ├─ [Overlay] MarkersContainer ⭐ (Name this!)
   ├─ [Border] TitleBorder
   │  └─ [TextBlock] TitleText
   └─ [Button] CloseButton
      └─ [TextBlock] CloseText
```

### 4D: Implement OnMapRefreshed Event (Graph Tab)

1. Switch to **Graph** tab
2. In **Functions** section, override `OnMapRefreshed`
3. Implement the event:

```
┌─────────────────────────────────────────────────────────────┐
│ Event Graph: OnMapRefreshed                                 │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────┐                               │
│  │ Event OnMapRefreshed     │                               │
│  ├──────────────────────────┤                               │
│  │ Markers (Array)          │───┐                           │
│  │ Player Location (Vector) │   │                           │
│  └──────────────────────────┘   │                           │
│                                   │                           │
│                                   ▼                           │
│  ┌─────────────────────────────────────────┐                │
│  │ Clear Children (MarkersContainer)       │                │
│  └─────────────────────────────────────────┘                │
│                      │                                       │
│                      ▼                                       │
│  ┌─────────────────────────────────────────┐                │
│  │ ForEach Loop (Markers)                  │                │
│  ├─────────────────────────────────────────┤                │
│  │ Array Element ──→ Create Marker Widget  │                │
│  │                   Convert World to Map   │                │
│  │                   Add to MarkersContainer│                │
│  └─────────────────────────────────────────┘                │
│                      │                                       │
│                      ▼                                       │
│  ┌─────────────────────────────────────────┐                │
│  │ Create Player Marker                    │                │
│  │ Add to MarkersContainer                 │                │
│  └─────────────────────────────────────────┘                │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

**Detailed Blueprint Nodes:**

```
OnMapRefreshed
│
├─ Get MarkersContainer (Variable)
│  └─ Clear Children
│
├─ ForEach Loop
│  ├─ Array: Markers (from event)
│  └─ Body:
│     ├─ Create Widget (WBP_MapMarker)
│     ├─ Set Marker Data
│     │  ├─ MarkerType ← Array Element.MarkerType
│     │  ├─ MarkerName ← Array Element.MarkerName
│     │  └─ Location ← Array Element.WorldLocation
│     ├─ WorldToMapPosition (Custom Function)
│     │  └─ Returns: Canvas Position
│     └─ Add to MarkersContainer
│        └─ Set Position in Slot
│
└─ Create Player Marker
   ├─ Create Widget (WBP_MapMarker)
   ├─ Set as Player Marker (bIsPlayer = true)
   ├─ WorldToMapPosition (PlayerLocation)
   └─ Add to MarkersContainer
```

### 4E: WorldToMapPosition Function

Create a custom function to convert world coordinates to map UI coordinates:

```
Function: WorldToMapPosition
├─ Input: WorldLocation (Vector)
└─ Output: MapPosition (Vector2D)

Implementation:
┌──────────────────────────────────────────────┐
│ WorldLocation                                │
│       │                                      │
│       ├─ Get X ──→ Normalize (Min=-50000,   │
│       │            Max=50000) ──→ * MapWidth │
│       │                                      │
│       └─ Get Y ──→ Normalize (Min=-50000,   │
│                    Max=50000) ──→ * MapHeight│
│                                              │
│  ┌────────────────────────────────────────┐ │
│  │ Make Vector2D(MapX, MapY)              │ │
│  └────────────────────────────────────────┘ │
│                │                             │
│                └──→ Return MapPosition       │
└──────────────────────────────────────────────┘
```

---

## Step 5: Create Marker Widget (Optional but Recommended)

**Location**: `Content/Core/Blueprints/UI/Widgets/WBP_MapMarker`

### Simple Marker Design:

```
┌─────────────────────────────────────────┐
│ WBP_MapMarker                           │
├─────────────────────────────────────────┤
│                                         │
│  Overlay (Root)                         │
│   │                                     │
│   ├─ Image (MarkerIcon)                │
│   │   - Size: 32x32                    │
│   │   - Anchor: Center                 │
│   │                                     │
│   └─ TextBlock (MarkerLabel)           │
│       - Text: Bind to MarkerName       │
│       - Position: Below icon           │
│                                         │
└─────────────────────────────────────────┘
```

**Add Variables:**
- `MarkerType` (EMapMarkerType)
- `MarkerName` (String)
- `bIsPlayer` (Boolean)

**Pre-Construct Event:**
```
Pre Construct
│
├─ Branch (bIsPlayer)
│  ├─ True: Set Icon to Player Icon (Blue)
│  └─ False: Switch on MarkerType
│     ├─ POI: Set Icon to POI Icon (Yellow)
│     └─ Gatherable: Set Icon to Resource Icon (Green)
│
└─ Set MarkerLabel Text to MarkerName
```

---

## Testing Checklist

After completing all steps:

- [ ] Compile Blueprint
- [ ] PIE (Play in Editor)
- [ ] Press `M` - Map should open
- [ ] See mouse cursor
- [ ] Player marker visible (blue dot)
- [ ] Press `M` again - Map should close
- [ ] Mouse cursor hidden

**For Discovery Testing:**
- [ ] Place `ADiscoverableMapLocation` in level
- [ ] Set properties (Type, Name, Radius)
- [ ] Approach the location
- [ ] Open map - Marker should appear

---

## Common Blueprint Issues

### Issue: Can't find DisplayMap in Action Name
**Solution**: Make sure the C++ code is compiled. Regenerate Visual Studio project files.

### Issue: Can't find MapWidget parent class
**Solution**: 
1. Close Unreal Editor
2. Right-click `.uproject` → Generate Visual Studio project files
3. Compile in Visual Studio or Rider
4. Reopen Unreal Editor

### Issue: OnMapRefreshed event not showing
**Solution**: Make sure parent class is set to `MapWidget`, not `UserWidget`

### Issue: Map opens but no markers
**Solution**: Check that `OnMapRefreshed` event is implemented and actually creates widgets

---

## Quick Verification

After setup, your project should have:

**Assets:**
- ✅ `IA_Map` (Input Action)
- ✅ `WBP_MapWidget` (Widget Blueprint, parent: MapWidget)
- ✅ `WBP_MapMarker` (Widget Blueprint, optional)
- ✅ Input Mapping has `M` → `IA_Map`

**Configuration:**
- ✅ BP_CustomPlayerController:
  - IA_Setup includes `IA_Map` → `DisplayMap`
  - MapWidgetClass = `WBP_MapWidget`

**Implementation:**
- ✅ WBP_MapWidget:
  - Parent class: MapWidget
  - OnMapRefreshed implemented
  - UI designed with markers container

---

## Next Level: Advanced Features

Once basic implementation works:

1. **Add Zoom**: Mouse wheel to scale MapContainer
2. **Add Pan**: Drag to move map
3. **Add Tooltips**: Hover over markers for info
4. **Add Legend**: Show marker types
5. **Add Filters**: Toggle marker visibility
6. **Add Fast Travel**: Click markers to teleport

---

**For complete text instructions**: See `MAP_IMPLEMENTATION_GUIDE.md`
