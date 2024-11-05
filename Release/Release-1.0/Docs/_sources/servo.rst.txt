servo
===================

The `Servo` class represents a servo device with specific parameters.

.. py:class:: devices.servo(port: int)

   A servo object.

   :param port: The port number the servo is connected to.
   :type port: int

   .. py:method:: set(speed: int, angle: int)

      Sets the speed and angle for the servo.

      :param speed: Speed of the servo movement.
      :type speed: int
      :param angle: Target angle for the servo.
      :type angle: int


   **Ports**
         
   .. py:attribute:: S1

      Servo port 1.

   .. py:attribute:: S2

      Servo port 2.