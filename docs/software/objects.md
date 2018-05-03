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
| **send**        |  *s*  | Send message to matching receive object                 |
| **mtof**        |       | Convert midi notes to frequencies                       |
| **number~**     |       | Display audio samples values                            |
| **number**      |       | Display a nuumber                                       |
| **select**      |       | Ouput bang if receive matching input                    |
| **snapshot~**   |       | Ouput sample signal values                              |
| **random**      |       | Ouput random number in a uniform distribution           |
| **unpack**      |       | Access elements in a list                               |
| **pack**        |       | Create and outputs a list of messages                   |
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
| **slider**      |       | Graphical slider sending values between 0 and 1         |
| **toggle**      |       | A switch button                                         |
| **bang**        |       | Send a bang message                                     |
| **meter~**      |       | A peak amplitude vu meter                               |
| **trigger**     |  *t*  | Trigger events based on received data                   |
| **newbox**      |       | Empty new box                                           |
| **errorbox**    |       | Error box                                               |
| **+**           |       | Add two numbers                                         |
| **\***          |       | Multiply two numbers                                    |
| **print**       |       | Print messages into Kiwi console                        |
| **receive**     | *r*   | Receive messages from a sender bound to the same name   |
| **loadmess**    |       | Output arguments as message when the patch is loaded    |
| **delay**       |       | Delay messages as bang                                  |
| **pipe**        |       | Delay any message                                       |
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
| **switch**      |       | Output message receive in selected inlet                |
| **gate~**       |       | Route received signal to selected output                |
| **switch~**     |       | Ouput signal received in selected inlet                 |
| **clip**        |       | Clip a number between a maximum and a minimum value     |
| **clip~**       |       | Clip a signal between a maximum and a minimum value     |
| **float**       |       | Store a float value                                     |
