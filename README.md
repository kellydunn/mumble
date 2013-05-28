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

A monome app that enables your monome to send midi messages to midi-enabled devices.

I've also had some success using my chronome to control midi devices!  If it uses `libmonome`, this app might work with it! :D

# why

I want to be able to use my monome with the mutable-instrument's shruti, and other MIDI-driveable things

# deps

  - libmonome
  - lphread
  - lyaml
  - lcheck (for testing)

# installation

Still investigating the prefered build tools of the C-World, but as of now, I have a custom Makefile that blindly uses the dependencies listed above.  This has only been tested for building in Arch Linux, so tread carefully :3

```
make
sudo make install
```
This will place mumble in `/usr/bin`, so be sure to include that in your `$PATH`.  In the future, I hope to add a `--prefix` option so that users may be able to configure their build path.

# usage

You can configure mumble to use the monomes and midi devices you so desire by creating a `~/.mumble/config.yml` file:

```
# in ~/.mumble/config.yml
monome_path: "osc.udp://127.0.0.1:12345/monome"
midi_device: "/dev/snd/midiC1D1"
```

# tests

Mumble currently uses the super rad `check` library to keep itself afloat.  Test coverage is still a bit shaky, but if you would like to run the tests, feel free to do so with the following command:

```
make test
```

# roadmap

  - provide different key mappings (like different scales, etc)
  - provide the ability to customize the midi muxer via a configuration file / potentially python bindings?
  - provide more interesting midi controls like pitch bending / volume control, etc.

# development

Nothing beats testing on a real midi device, but if you're in a pinch, you can virutualize your midi setup by following the steps below.  There are other ways to achieve this, but I've found that a combination of `timidity` and `aconnect` work pretty well!

```
# A way to virutalize MIDI ports
timidity -iA &
sudo modprobe snd-virmidi # you may need to reload this kernel module if you're on Arch Linux.

# Connecting Virtual MIDI ports to other MIDI-consumable services (like Hydrogen)
# aconnect <midi-through-port-address>:0 <timidity-forward-port-address>:0
aconnect 14:0 20:0
<edit mumble configuration to point to /dev/snd/midiC1D0 or something similar>
```
