Also see [TODO.md in BabelWires](https://github.com/Malcohol/BabelWires/blob/main/TODO.md)

BabelWires-Music:
* Track's cache is not thread-safe.
* Support other formats
* Improved handling of event truncation, to allow events to traverse looped boundaries.
  - Use new group events to denote truncated end and start. 
  - When truncated end meets truncated start, act as though event was not truncated.
* TrackEvent::Category should be an identifier.

Seq2tape:
* Add support for setting name and copyright from commandline.
* Add another text field: info.
* Test Multichannel input and output
* Consider implementing basic WAV support myself, so project works without libsnd.

SMF
* Time signature
* Instruments
* Support Standard MIDI event types
* Yamaha XF chords
* Support Format 2 files.

Processors:
* First note, last note - Can be combined with the excerpt processor (and possibly quantize) to trim a track.
* Split by event category - Build a record of tracks by category
  - Would need category to be an identifier (see above)
  - Record type constructors (See BabelWires PR) OR registry of categories.
* GetSetOfChords
* ChordSequencer