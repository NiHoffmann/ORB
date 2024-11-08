# Tools und Referenzliste

### Tools und Referenzen
 
| Tool            | Version       |     Kommentar | 
| -------------   | ------------- | ------------- |
| [Git](https://git-scm.com/)             |  2.38.1.windows.1 ||
| [Code::Blocks](https://www.codeblocks.org/)    | 20.03             ||
| [EmBitz](https://www.embitz.org/)        |  2.50             ||
| Python          | [Python 3.11.4](https://www.python.org/downloads/release/python-3114/) ||
| [MSYS2](https://www.msys2.org/)           | 20240507      | MSYS2 will be used to install gcc and all the needed tools, such as `make` and `GNU coreutils`.|
| GCC             | 13.2.0 (Rev3, Built by MSYS2 project) | installed by MSYS2|
| Make            | GNU Make 3.81 | installed by MSYS2 |
| [DFU-Util](https://dfu-util.sourceforge.net/)        |     0.7 ||
| [zadig](https://zadig.akeo.ie/)           |     2.9 ||
| [Sphinx](https://www.sphinx-doc.org/)          |   8.0.2 ||
| [chocolatey](https://chocolatey.org/) | 2.2.0||
| ORB-Firmware    |[https://github.com/ThBreuer/ORB-Firmware](https://github.com/ThBreuer/ORB-Firmware/) <br> Commit: [e8d74fb](https://github.com/ThBreuer/ORB-Firmware/commit/e8d74fba21e8ca6ec5e522813ab1235f87808662) | This Repository was integrated into this Project, and added as a Submodule.| 
| ORB-Application |[https://github.com/ThBreuer/ORB-Application](https://github.com/ThBreuer/ORB-Application) <br>Commit: [f0a4fbb](https://github.com/ThBreuer/ORB-Application/commit/f0a4fbb16f6ff9cc55ac2eb13b818f4c39970ef4)|This Repository was integrated into this Project, and added as a Submodule.|
| MicroPython     |[https://github.com/micropython/](https://github.com/micropython/) <br> Commit: [e9814e9](https://github.com/micropython/commit/e9814e987bcc816fb67e38748a5afce466c45606)|This Repository was integrated into this Project, and added as a Submodule.|
| Mpy-Cross       | [v6.3](./../../../../micropython/tree/master/mpy-cross)| Part of the MicroPython Repository |
| Python-Intelhex | [https://github.com/python-intelhex/intelhex](https://github.com/python-intelhex/intelhex)<br> Commit: [6d0e826](https://github.com/python-intelhex/intelhex/commit/6d0e826b1efd0c9978e0027603138c5a436729a3) ||
| bin2hex.py      | [https://gist.github.com/pavel-a/89d71b3aba9d7a9e6f8a61d728b08a8e](https://gist.github.com/pavel-a/89d71b3aba9d7a9e6f8a61d728b08a8e)||

### Literatur und Referenzen
| Kürzel | Autor | Quelle |
|    -   |  -  |   -   |
| ORB-FW | Thomas Breuer. : <br><br><br> |  „ORB-Firmware“. <br> [https://github.com/ThBreuer/ORB-Firmware](https://github.com/ThBreuer/ORB-Firmware/) <br> Commit: [e8d74fb](https://github.com/ThBreuer/ORB-Firmware/commit/e8d74fba21e8ca6ec5e522813ab1235f87808662)|
| ORB-APP | Thomas Breuer. : <br><br><br> |  „ORB-Application“. <br> [https://github.com/ThBreuer/ORB-Application](https://github.com/ThBreuer/ORB-Application) <br>Commit: [f0a4fbb](https://github.com/ThBreuer/ORB-Application/commit/f0a4fbb16f6ff9cc55ac2eb13b818f4c39970ef4)|
| MP     | Damien P. George, Paul Sokolovsky et al. : <br><br><br>| „MicroPython“. <br> [https://github.com/micropython](#https://github.com/micropython). <br> Commit: [e9814e9](https://github.com/micropython/commit/e9814e987bcc816fb67e38748a5afce466c45606). <br> Datum: 16.08.2024.  |
| MPD| Damien P. George, Paul Sokolovsky et al. : <br> <br> | „Implementing a Module“. <br> [https://docs.micropython.org/en/latest/develop/library.html](https://docs.micropython.org/en/latest/develop/library.html) <br> Stand: 23.10.2024. |  |
| MPC | Peter Hinch. : <br> <br> <br> | „Exit micropython from interrupt in c“. <br> https://forum.micropython.org/viewtopic.php?t=2521#p14831 <br> Datum: 17.10.2016.|  
| ARM-al | Hrsg.: Arm Limited : <br><br><br><br> | „Basic data types“. <br> [https://developer.arm.com/documentation/dui0491/i/C-and-C---Implementation-Details/Basic-data-types](https://developer.arm.com/documentation/dui0491/i/C-and-C---Implementation-Details/Basic-data-types) <br> Stand: 30.10.2024 |
| C-s | Hrsg.: ©ISO/IEC, <br> Ballot-Version, <br> Zugriff über: The University of Western Australia.: <br>| „Programming languages — C“.<br> [ISO/IEC 9899:2017 - Ballot C17](https://teaching.csse.uwa.edu.au/units/CITS2002/resources/n2176.pdf) <br> [https://teaching.csse.uwa.edu.au/units/CITS2002/resources/n2176.pdf](https://teaching.csse.uwa.edu.au/units/CITS2002/resources/n2176.pdf) <br> Jahr: 2017. <br><br>| 
| - |Hrsg.: Arm Limited.: <br><br><br>|  „Arm® Cortex®-M4 Processor Technical Reference Manual“. <br> https://documentation-service.arm.com/static/5f19da2a20b7cf4bc524d99a <br> Stand: 02.Mai.2010|
|- |Joseph Yiu. : <br><br><br>| „The Definitive Guide to ARM® Cortex®-M3 and Cortex®-M4 Processors“. <br> ISBN: 9780124079182 <br> Datum: 06.10.2013|
| - |Jonathan W. Valvano.:<br><br><br>| „Embedded Systems: Real-Time Operating Systems for Arm Cortex M Microcontrollers“. <br> ISBN: 978-1466468863 <br> Datum: Januar 2017   |
| GCC-cf |Hrsg.:  Free Software Foundation, Inc.:<br><br>| „3.11 Options That Control Optimization“. <br> [https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) <br> Stand: 31.10.2024 |
| GCC-co |Hrsg.:  Free Software Foundation, Inc.:<br><br>| „6.3.3.2 Compilation options“. <br> [https://gcc.gnu.org/onlinedocs/gnat_ugn/Compilation-options.html](https://gcc.gnu.org/onlinedocs/gnat_ugn/Compilation-options.html) <br> Stand: 31.10.2024 |
| - |Hrsg.: STMicroelectronics.: <br><br><br><br><br>|  „STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439 advanced Arm®-based 32-bit MCUs“. <br> https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf<br> Datum: Juni 2024|
| STM |Hrsg.: STMicroelectronics.: <br><br><br><br>|  „STM32 Cortex®-M4 MCUs and MPUs programming manual“. <br> https://www.st.com/resource/en/programming_manual/pm0214-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf<br> Datum: März 2024|   
| AMD | Hrsg. Advanced Micro Devices, Inc. <br><br><br>| „AMD64 Architecture Programmer’s Manual Volume 1: Application Programming“. <br> [https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/24592.pdf](https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/24592.pdf) <br> Datum: Oktober 2020 |