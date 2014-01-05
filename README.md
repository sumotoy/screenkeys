screenkeys
==========

Hardware & Software for drive 1..64 LCD Screenkeys LC button with Teensy3.x

Screenkeys LCD push buttons are expensive and nasty to use because they have a weird serial protocol that use a strange parity bit
and they need to be constantly clocked or they will damaged. I just find a library attemp on the net but it can drive just one button
and was too confused (library uses some vars from sketch and viceversa).
The product application notes where old and incomplete and uses old stuff like shift resisters that I usually avoid expecially when working with audio since they create some interferences.
Screenkeys have just 6 wires each, many can be paralled, just 2 need to be separated. They use a clock from 40Khz to 4Mhz that should be constantly
applied and data protocol should be applied in the right sequence with clock and need several start bit,end bit and parity check
that should be calculated at every data stream.
The Switch itself it's pretty amazing, you have a push button with a graphical LCD and programmable background that allow you
to create interfaces that can be seen only in Top video studio's, NASA, Avionics.
The first problem I had to solve it's a way to program many of these little beast at once with a tiny microcontroller by using
as much less resources and wires. Forget Arduino uno et similar, they simply have insufficent resources, so I choosed one
of my preferred Micro around, Teensy3.x from <b>Paul Stoffregen</b> http://www.pjrc.com/store/teensy3.html that it's a 32bit
tiny microcontroller with a lot of features and much better supported than Arduino Due that it's gigantic, less powerful and expensive.
I've used several SPI GPIO's from Microchip MCP23s17 with theyr exclusive HAEN feature to save pins and my universal
GPIO library, together with a couple of level converter chip and a driver plus some condenser and resistors. Schematics will be published soon but it's surprisely uncomplicated even for 64 buttons. I have choosed single button scan and not a matrix (that uses much less gpio's) because I want to keep the ability to identify multiple buttons at once.
With this hardware you have a choose to use internal generated microcontroller lcd clock generation or even use an external one to save a pin and some CPU resources.
I have tested library and hardware with 48 switches and works like a charm, it can support max 64 switches.
With 48 switches (this means 48 tiny LCD screens!!!) Teensy3 resources are used at 1% since I'm using as much I can hardware features
of this chip, for example key scan it's triggered by an IRQ so if you are not touching keyboard the cpu loop it's almost empty
and can be used for many other useful and fun stuff.
I choosed a clock frequency of 1Mhz to prevent interferences with audio if you plan to use as interface for audio stuff.
The library will initially works with LC16 and LC24 series (I just have these) but can be expanded for other models.
Be free to use for your purposes but not for any commercial or military applications.

