# Objects

## Help patches

A list of help patches describing how each object works is <a href="../ressources/pathces/help/help.zip" target='_blank_'>here</a>. The list of help patches is currently incomplete but will evolve later on.

## List of objects

| Object          | Alias |                       Description                       |
|-----------------|:-----:|---------------------------------------------------------|
| **line~**       |       | Generate an audio ramp signal                           |
| **noise~**      |       | A white noise generator                                 |
| **sah~**        |       | Samples and holds an input signal                       |
| **hub**         |       | Send message to other users                             |
| **send**        |  *s*  | Send messages to a receiver bound to the same name      |
| **mtof**        |       | Convert midi notes to frequencies                       |
| **number~**     |       | Display audio samples values                            |
| **number**      |       | Display and output number                               |
| **select**      | *sel* | Output bangs based on input matching                    |
| **snapshot~**   |       | Output sample signal values                             |
| **random**      |       | Generate a random number                                |
| **unpack**      |       | Breaks a list into individual atoms                     |
| **pack**        |       | Create a list from individual atoms                     |
| **comment**     |       | Add a comment to the patch                              |
| **!=~**         |       | Signal inequality operator                              |
| **==~**         |       | Signal equality operator                                |
| **>=~**         |       | Signal superior operator                                |
| **>~**          |       | Signal strict superior operator                         |
| **<=~**         |       | Signal inferior operator                                |
| **<~**          |       | Signal strict inferior operator                         |
| **/~**          |       | Divide two signals                                      |
| **-~**          |       | Substract two signals                                   |
| **%**           |       | Compute the modulo of number in desired basis           |
| **pow**         |       | Compute exponentiation of a number                      |
| **==**          |       | Equality operator                                       |
| **!=**          |       | Inequality operator                                     |
| **>=**          |       | Superior operator                                       |
| **>**           |       | Strict superior operator                                |
| **<=**          |       | Inferior operator                                       |
| **<**           |       | Strict inferior operator                                |
| **/**           |       | Compute the division of two numbers                     |
| **-**           |       | Compute the substraction of two numbers                 |
| **message**     |       | Send messages or list of messages                       |
| **slider**      |       | GUI sending values between 0. and 1.                    |
| **toggle**      |       | GUI sending 0 or 1                                      |
| **bang**        |       | GUI sending a bang message                              |
| **meter~**      |       | A Peak Program Meter GUI                                |
| **trigger**     |  *t*  | Trigger events based on received data                   |
| **newbox**      |       | Empty new box                                           |
| **errorbox**    |       | Error box                                               |
| **+**           |       | Add two numbers                                         |
| **\***          |       | Multiply two numbers                                    |
| **print**       |       | Print messages into Kiwi console                        |
| **receive**     | *r*   | Receive messages from a sender bound to the same name   |
| **loadmess**    |       | Output arguments as message when the patch is loaded    |
| **delay**       |       | Delays messages as bang                                 |
| **pipe**        |       | Delays any message                                      |
| **metro**       |       | Output bang at regular time interval                    |
| **adc~**        |       | Receive audio signals from inputs or external devices   |
| **dac~**        |       | Send audio signals to outputs or external devices       |
| **osc~**        |       | A cosine oscillator                                     |
| **phasor~**     |       | Generate a sawtooth signal                              |
| **\*~**         |       | Multiply two signals                                    |
| **+~**          |       | Add two signals                                         |
| **sig~**        |       | Convert numbers into audio signals                      |
| **delaysimple~**|       | Simple variable delay line with optional feedback value |
| **scale**       |       | Convert a number from one range into another range      |
| **gate**        |       | Route message to selected output                        |
| **switch**      |       | Output messages received in selected inlet              |
| **gate~**       |       | Route received signal to selected output                |
| **switch~**     |       | Output signal received in selected inlet                |
| **clip**        |       | Clip a number between a maximum and a minimum value     |
| **clip~**       |       | Clip a signal between a maximum and a minimum value     |
| **float**       |       | Store a float value                                     |
