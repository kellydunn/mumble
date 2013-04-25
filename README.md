```
                              __       ___             
                             /\ \     /\_ \            
  ___ ___   __  __    ___ ___\ \ \____\//\ \      __   
/' __` __`\/\ \/\ \ /' __` __`\ \ '__`\ \ \ \   /'__`\ 
/\ \/\ \/\ \ \ \_\ \/\ \/\ \/\ \ \ \L\ \ \_\ \_/\  __/ 
\ \_\ \_\ \_\ \____/\ \_\ \_\ \_\ \_,__/ /\____\ \____\
 \/_/\/_/\/_/\/___/  \/_/\/_/\/_/\/___/  \/____/\/____/
                                                       
monome + midi
                                                  
```

# what 

a derpy little utility that helps monomes talk midi

# why

I want to be able to use my monome with the mutable-instrument's shruti, and other MIDI-driveable things

# deps

  - libmonome
  - lphread

# installation

Still investigating the prefered build tools of the C-World, but as of now, I have a custom Makefile that blindly uses the dependencies listed above.  This has only been tested for building in Arch Linux, so tread carefully :3

```
make
sudo make install
```
This will place mumble in `/usr/bin`, so be sure to include that in your `$PATH`.

# usage

You can configure mumble to use the monomes and midi devices you so desire:

```
# in ~/.mumble/config.yml
monome_path: "osc.udp://127.0.0.1:12345/monome"
midi_device: "/dev/snd/midiC1D1"
```

# tests

There are a few tests that help mumble stay above water.  You can run them if you like!

```
make test
./bin/*_test
```

# roadmap

  - provide different key mappings (like different scales, etc)
  - provide the ability to customize the midi muxer via a configuration file / potentially python bindings?
  - provide a way to customize default midi device via a configuration file
  - provide more interesting midi controls like pitch bending / volume control, etc.

# development

I'm currently using Arch Linux, so creating midi pipes for the sake of development requires some setup. 

Timidity is a great application to help with virtualizing MIDI so that you may allow mumble to talk to DAWs like Rosegarden, Renoise, or even Ableton.

```
# A way to virutalize MIDI ports
timidity -iA  
sudo modprobe snd-virmidi

# Connecting Virtual MIDI ports to other MIDI-consumable services (like Hydrogen)
# aconnect <midi-through-port-address>:0 <timidity-forward-port-address>:0
aconnect 14:0 20:0
<edit mumble configuration to point to /dev/snd/midiC1D0 or something similar>
```