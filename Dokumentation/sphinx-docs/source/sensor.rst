sensor
===================

The `Sensor` class represents a sensor device with configurable parameters and multiple data retrieval methods.

.. py:class:: devices.sensor(port: int, type: int, mode: int, option: int)

   Initializes a sensor instance connected to a specified port with defined type, mode, and optional parameters.

   :param port: The port number the sensor is connected to.
   :type port: int
   :param type: Type of the sensor, such as `Analog`, `I2C`, `TOF`, `Touch`, or `UART`.
   :type type: int
   :param mode: Operating mode of the sensor.
   :type mode: int
   :param option: Additional configuration option.
   :type option: int

   .. py:method:: config(type: int, mode: int, option: int)

      Configures the sensor.

      :param type: Type of the sensor (`Analog`, `I2C`, `TOF`, `Touch`, or `UART`).
      :type type: int
      :param mode: Operating mode of the sensor.
      :type mode: int
      :param option: Additional configuration option.
      :type option: int

   .. py:method:: get() ->  SensorReport

      Retrieves the sensor report as a dictionary representation.

   .. py:method:: getDigital() -> int

      Returns a digital reading from the sensor.

      :returns: The digital value.
      :rtype: int

   .. py:method:: getValueExt(channel: int) -> int

      :param channel: The channel number to retrieve the value from.
      :type channel: int
      :returns: The value from the specified channel.
      :rtype: int

**Ports**
   .. py:attribute:: S1 = 0

      Sensor port 1.

   .. py:attribute:: S2 = 1 

      Sensor port 2.

   .. py:attribute:: S3 = 2

      Sensor port 3.

   .. py:attribute:: S4 = 3

      Sensor port 4.

**Types**

   .. py:attribute:: Analog = 0

      Analog sensor type.

   .. py:attribute:: I2C = 1

      I2C sensor type.

   .. py:attribute:: TOF = 2

      Time-of-Flight sensor type.

   .. py:attribute:: Touch = 3

      Touch sensor type.

   .. py:attribute:: UART = 4

      UART sensor type.

.. py:class:: devices.SensorReport

   A `SensorReport` is a read-only representation of a single sensor reading.  
   It can't be instantiated manually and is only returned by the `get()` method.

   **Attributes:**

   .. py:attribute:: values
      :type: list[int]
      
      An array containing two elements representing sensor data.

   .. py:attribute:: type
      :type: int

      The type of sensor.

   .. py:attribute:: options
      :type: int

      Additional configuration options.

   .. py:attribute:: lenExpt
      :type: int

   
   