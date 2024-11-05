time
===========

The `time` module provides utilities for working with time, including retrieving the current time and introducing delays.

.. py:function:: time.getTime() -> int

   Returns the time, since microcontroller start.

   :returns: The current time in milliseconds.
   :rtype: int

.. py:function:: time.wait(ms: int)

   Pauses execution for a given number of milliseconds.

   :param ms: Number of milliseconds to wait.
   :type: int
