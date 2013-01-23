```
                              __       ___             
                             /\ \     /\_ \            
  ___ ___   __  __    ___ ___\ \ \____\//\ \      __   
/' __` __`\/\ \/\ \ /' __` __`\ \ '__`\ \ \ \   /'__`\ 
/\ \/\ \/\ \ \ \_\ \/\ \/\ \/\ \ \ \L\ \ \_\ \_/\  __/ 
\ \_\ \_\ \_\ \____/\ \_\ \_\ \_\ \_,__/ /\____\ \____\
 \/_/\/_/\/_/\/___/  \/_/\/_/\/_/\/___/  \/____/\/____/
                                                       
oscmidimumblbmmfbbt~-..
                                                  
```

# what 

a derpy little utility that helps monomes talk midi

# why

I want to be able to use my monome with the mutable-instrument's shruti!

# deps

  - libmonome

# roadmap

  - provide different key mappings
  - provide the ability to customize the midi muxer
  - provide more interesting midi controls like the following:

```
+---+---+---+---+---+---+---+---+
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
+---+---+---+---+---+---+---+---+

0: vel down
1: vel up
2: key down
3: key up
4: ??? (record midi file?)
5: ??? (record midi file?)
6: pitch bend down (press enabled; duration for monome or pressure for chronome)
7: pitch bend up   (press enabled; duration for monome or pressure for chronome)

```

# development

I'm currently using arch linux, so creating midi pipes for the sake of development requires some setup:

```
timidity -iA
sudo modprobe snd-virmidi
aconnect <midi-through-port-address>:0 <timidity-forward-port-address>:0
<edit mumble configuration to point to /dev/snd/midiC1D0 or something similar>
```