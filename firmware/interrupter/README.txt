These are the GPL'd source files for the oneTesla MIDI interrupter. To edit the source
(interrupter.ino) you will need the Arduino IDE. You can flash interrupter.hex with your favorite
tool - we use Atmel Studio and a AVR ISP MkII. The binaries are compatible with the 16MHz ATMega
328 microcontroller; the source has timing settings and registers compatible with the same
microcontroller, but with some modification should be able to be coaxed to run on similar micros.

For more documentation (including a walkthrough) please refer to the interrupter guide found at
http://www.onetesla.com, under the "Downloads" section.

*CHANGES*

0.5.0 (unreleased): Initial version, monophonic only
0.8.0 (unreleased): Initial polyphony implementation
0.9.0 (unreleased): Bugfix release
1.0.0: First public version. Added support for variable power via pulse-width control.
1.2.0: Added support for standalone (fixed-frequency) mode; pinouts changed to match new PCB.
1.3.0: Added support for velocity; fixed a timer bug.
1.3.1: Fixed an integer math bug which prevented proper operation on high notes.
1.3.2: Added (hard-coded) velocity mapping, which is currently linear.
1.4.0: Added sustain support. There is also a hidden programming mode, but it is highly experimental and
       does not work with the stock oneTesla dongles or the M-Audio Midisport UNO.
1.4.5: Significant bugfixes. Added support for pitch bend.
1.4.9: Bugfix release.
1.5.0: Hidden programming mode removed. Restructured code in an attempt to make it more portable.
1.6.0: Added active note buffer for more efficient playing of notes. Significant internal restructuring.
1.6.5: Added support for power envelope waveforms; among other things, this helps reduce flashover on low notes.

*LICENSING*
All files contained herein are licensed under the GNU General Public License Version 3.0