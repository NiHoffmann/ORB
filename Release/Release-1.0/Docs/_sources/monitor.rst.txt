monitor
==============

The `monitor` module provides functionality for ORB-Monitor communication.

.. py:function:: monitor.getKey() -> int

   :returns: Current key pressed on the monitor.
   :rtype: int

.. py:function:: monitor.setText(text: str)

   Sets the text to be displayed on the monitor. At the Moment only 32 characters are supported for one Line.

   :param text: The text to display.
   :type text: str

.. py:class:: monitor.keys

   A class representing key constants for the `monitor`.

   Each key constant has a unique integer value.  

   **Available keys**:

   - **NO_KEY** = 0
   - **A1** = 1
   - **A2** = 2
   - **A3** = 3
   - **A4** = 4
   - **A5** = 5
   - **A6** = 6
   - **A7** = 7
   - **A8** = 8
   - **B1** = 9
   - **B2** = 10
   - **B3** = 11
   - **B4** = 12
   - **B5** = 13
   - **B6** = 14
   - **B7** = 15
   - **B8** = 16
   - **B9** = 17
   - **B10** = 18
   - **B11** = 19
   - **B12** = 20
   - **C1** = 21
