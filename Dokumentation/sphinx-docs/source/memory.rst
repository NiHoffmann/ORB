memory
=============

The `memory` module provides functions to manage memory operations.

.. py:function:: memory.setMemory(addr: int, data: list | bytes)

   Sets memory at the specified address with the given data.

   :param addr: The memory address to set.
   :type addr: int
   :param data: The data to be stored, which can be a list or bytes.
   :type data: list | bytes

.. py:function:: memory.getMemory(addr: int, length: int) -> bytes

   Returns memory starting from the specified address in the user-memory-region.

   :param addr: The starting memory address.
   :type addr: int
   :param length: The number of bytes to retrieve.
   :type length: int
   :returns: The retrieved memory content as bytes.
   :rtype: bytes

.. py:function:: memory.clearMemory()

   Clears all stored memory data.
