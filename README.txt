ChaosConnect is the GUI application for interacting with the Electronic 
Chaos System.

This software was originally written for Windows XP and higher, but it 
has also been compiled on Linux. Work is in progress to get the software
working on Mac OSX.

GCC is the compiler that was used to build this. On Windows, that means
you need to grab MinGW. You will also need libusb-win32 and wxWidgets 2.8
in order to build this application.

In addition to these standard libraries, this application also requires a
copy of libchaos to be present in lib/. This can be built separately 
using GCC. It can be found on at http://github.com/chaoscircuit/libchaos.

Acknowledgements:

Thanks to Mark James for the Silk icon set used by some GUI elements
http://www.famfamfam.com/lab/icons/silk/

Thanks to Volodymyr Myrnyy for help on the FFT code
http://gfft.sourceforge.net/
http://www.ddj.com/cpp/199500857