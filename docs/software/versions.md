# Versions

Kiwi follows the [Semantic Versioning Specification](http://semver.org/).

## v1.0.2

#### New objects:
- [sf.play~]: play an audio file from disk.
- [sf.record~]: record an audio file on disk.

#### Improvements :
- All object boxes can now be resized.
- Changed the look of the gui links and boxes.
- Enhance contrast between local and distant selections.
- Add contrast between control and signal pins.
- Add a dialog box to set the name of the patch to create.
- Drag-and-drop a file to the console to open it.
- Improved client/server version check.

#### Bug-fix
- Fix crashes in transaction stack.
- Handle 404 errors in *download* and *duplicate* API requests.
- Fix connected user list display

#### Dev:
- Bump Kiwi Model version to v4.0.1
- Removes "ratio", "min_width", "min_height" members from the `model::Object`.
- Update model converter code.
- Use juce (v5.2.1).
- Add [kiwi-node-server](https://github.com/Musicoll/kiwi-node-server) as submodule.
- Add Juce dependency to the engine.

## v1.0.1

[select] :
- fix bad output when symbol match
- output input if doesn't match (was bang)
- Add [sel] alias

[number] :
- improve decimal drag
- Add patcherview shortcuts (f/i)
- Output only when value has changed

[number~] :
-  updated draw method

[bang] :
- Add set method (flash without output)

[random] : (breaking-changes)
- default range is 0 (was 100)
- output random value between 0 and range-1 (was 0 and range)
- default seed is 0 (was 1)
- a seed of 0 means that it is initialized with an unpredictable time value.
- seed can now be set
- Add a *seed* method and remove the seed inlet
- remove range inlet when the range argument is specified

misc:
- Allow undo/redo of [message] and [comment] text edition.
- Remove "kiwi::model" in error message.

## v1.0.0

- Fix numerous bugs.
- Add new required objects (gate/gate~/switch/switch~/float/clip/clip~).
- Improve former objects' behaviour.

## v1.0.0-beta

- Adding graphical objects.
- Enable collaborative control with object hub.
- Set of audio and control objects for creating audio processing tools.
- Improving message semantic with dollar and commas.
- User management.
    * Authentication.
    * Improve user experience workflow.
- Collaborative environment.
    * More document metadata.
    * Search document enabled.
- Enable patch compatibility between versions.
- Fix miscellaneous bugs.
- Add new objects.

## v0.1.0

- Update the client user interface, change application design (icons, menubars, tootips...)
- Removed automatic client discovery (Multicast service provider and explorer).
- Fix some collaboration relative bugs.
- Fix some Windows and Linux bugs.
- Add a suggestion popup with objects name when typing into a box and io tootips.
- Get and connect to remote documents with the Document browser window.
- Add a scheduler and make the objects pipe, delay and metro.
- Add an "About Kiwi" Window.
- Add an "Application preferences" window.
- Add new objects.

## v0.0.3

- Collaboration on LAN with automatic client discovery.
- Add DSP chain
- Add some basic DSP objects
- Add new objects.

## v0.0.2

- Add beacons and a beacon dispatcher window
- Add new objects.

## v0.0.1

First release (not functional).
