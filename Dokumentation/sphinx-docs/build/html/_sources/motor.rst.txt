motor
===================

The `Motor` class represents a motor device with configurable parameters and various modes of operation.

.. py:class:: devices.motor(port: int, direction: int, ticks: int, acc: int, kp: int, ki: int)

   Initializes a motor instance.

   :param port: The port that the motor is connected to.
   :type port: int
   :param direction: Direction of the motor (`FORWARD` or `REVERSE`.)
   :type direction: int
   :param ticks: Number of encoder ticks for one revolution.
   :type ticks: int
   :param acc: Acceleration rate of the motor.
   :type acc: int
   :param kp: Proportional gain for the motor control.
   :type kp: int
   :param ki: Integral gain for the motor control.
   :type ki: int

   .. py:method:: config(direction: int, ticks: int, acc: int, kp: int, ki: int)

      Configures the motor parameters.

      :param direction: Direction of the motor, either `FORWARD` or `REVERSE`.
      :type direction: int
      :param ticks: Number of encoder ticks for one revolution.
      :type ticks: int
      :param acc: Acceleration rate.
      :type acc: int
      :param kp: Proportional gain.
      :type kp: int
      :param ki: Integral gain.
      :type ki: int

   .. py:method:: set(mode: int, speed: int, position: int)

      Sets the motor's operating mode, speed, and position.

      :param mode: The mode in which to operate the motor (`POWER_MODE`, `BRAKE_MODE`, `SPEED_MODE`, or `MOVETO_MODE`).
      :type mode: int
      :param speed: The speed of the motor.
      :type speed: int
      :param position: The target position for the motor.
      :type position: int

   .. py:method:: get() -> Dict["speed": int, "power": int, "position": int]

      :returns: A dictionary containing:
         - **speed**: The current speed.
         - **power**: The current power.
         - **position**: The current position.

**Ports**

   .. py:attribute:: M1 = 0

      Motor port 1.

   .. py:attribute:: M2 = 1

      Motor port 2.

   .. py:attribute:: M3 = 2

      Motor port 3.

   .. py:attribute:: M4 = 3

      Motor port 4.

**Modes**

   .. py:attribute:: FORWARD = 1

   .. py:attribute:: REVERSE = -1

   .. py:attribute:: POWER_MODE = 0

   .. py:attribute:: BRAKE_MODE = 1

   .. py:attribute:: SPEED_MODE = 2

   .. py:attribute:: MOVETO_MODE = 3